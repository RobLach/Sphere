#include "SphLaser.h"
#include "SphMath.h"
#include "SphSdlInput.h"
#include "SphAvatar.h"
#include "SphGame.h"
#include "SphWorld.h"
#include "SphItem.h"
#include "SphSphericBound.h"
#include "SphCollisionDetection.h"
#include "SphAvatarController.h"
#include "SphAttachment.h"
#include "SphLaserGunAttachment.h"
#include "SphLegsAttachment.h"
#include "SphEnemy.h"

SphMesh* SphLaser::s_laserMesh = NULL;

//Create SphLaser.
SphLaser::SphLaser(){
	if(s_laserMesh == NULL){
		s_laserMesh = new SphMesh();
		s_laserMesh->Load("./Meshes/Projectiles/", "Laser.drkMesh");
	}
	this->m_mesh = s_laserMesh;
	this->m_damage = 35;
	this->m_speed = 25;
	this->m_shotType = Laser;
	this->m_lifetime = 0;
	this->m_range = 130;
	g_game.PlaySound("Audio/laser.ogg", 1.0f, false);
}

//Update SphLaser.
//seconds - since the last call.
void SphLaser::Update(float seconds){
	//Check to see if it has exceeded it's range
	if(this->m_lifetime > this->m_range){	
		m_alive = false;
	} else {		
		this->m_lifetime ++;

		float delta[N_XYZ];
		Subtract(delta, m_target, GetPosition());
		Normalize(delta);
		Multiply(delta, m_speed * seconds);
		Translate(delta);

		ProcessCollisions();
	}
}

//Get list of collisions for the Laser and send them for processing.
void SphLaser::ProcessCollisions(){
	SphCollisionsList collisionsList;
	g_game.m_world->GetCollisions(&collisionsList, m_bound);
	for(SphCollisionsList::iterator iter = collisionsList.begin(); iter != collisionsList.end(); iter++){
		SphCollisionInfo info = *iter;
		ProcessCollision(&info);
	}
}

//Process each collision.
void SphLaser::ProcessCollision(SphCollisionInfo* info){
	if((info->m_object)->GetObjType() == SphObject::ENEMY_OBJ){
		if( (info->m_object)->m_damagedByLaser ) {
			(info->m_object)->TakeDamage(this->GetDamage());
			if((info->m_object)->GetHealth() <= 0) {
				g_game.m_world->RemoveCollidable(info->m_object);
			}
		}
	}
	//Remove laser if it hit anything (besides the avatar and it's gun)
	if(!((info->m_object)->GetObjType() == SphObject::AVATAR_OBJ)) {
		m_alive = false;
	}
}