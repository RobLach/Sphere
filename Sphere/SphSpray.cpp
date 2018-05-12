#include "SphSpray.h"
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
#include "SphSprayGunAttachment.h"
#include "SphLegsAttachment.h"
#include "SphEnemy.h"

SphMesh* SphSpray::s_sprayMesh = NULL;

//Create SphSpray.
SphSpray::SphSpray(){
	
	if(s_sprayMesh == NULL){
		s_sprayMesh = new SphMesh();
		s_sprayMesh->Load("./Meshes/Projectiles/", "Laser.drkMesh");
	}
	this->m_mesh = s_sprayMesh;
	g_game.PlaySound("Audio/spray.ogg", 0.8f, false);
	this->m_damage = 5;
	this->m_speed = 50;
	this->m_shotType = Spray;
	this->m_lifetime = 0;
	this->m_range = 5;
	this->m_particles = new SphSprayParticles("./Textures/Particles/", "spray.tga", this->GetPosition());
}

//Update SphSpray.
//seconds - since the last call.
void SphSpray::Update(float seconds){
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
		this->m_particles->ComputeParticles(this->GetPosition(), g_game.m_camera->GetForward());
		ProcessCollisions();
	}
}

//Render Particles for this projectile
void SphSpray::Render(){
	this->m_particles->Render();
}

//Get list of collisions for the Spray and send them for processing.
void SphSpray::ProcessCollisions(){
	SphCollisionsList collisionsList;
	g_game.m_world->GetCollisions(&collisionsList, m_bound);
	for(SphCollisionsList::iterator iter = collisionsList.begin(); iter != collisionsList.end(); iter++){
		SphCollisionInfo info = *iter;
		ProcessCollision(&info);
	}
}

//Process each collision.
void SphSpray::ProcessCollision(SphCollisionInfo* info){
	if((info->m_object)->GetObjType() == SphObject::ENEMY_OBJ){
		if( (info->m_object)->m_damagedBySpray ) {
			(info->m_object)->TakeDamage(this->GetDamage());
			if((info->m_object)->GetHealth() <= 0) {
				g_game.m_world->RemoveCollidable(info->m_object);
			}
		}
	}
	//Remove spray if it hit anything (besides the avatar and it's gun)
	if(!((info->m_object)->GetObjType() == SphObject::AVATAR_OBJ)) {
		m_alive = false;
	}
}