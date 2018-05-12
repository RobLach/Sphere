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
#include "SphSprayGunAttachment.h"
#include "SphLegsAttachment.h"
#include "SphSprayParticles.h"
#include "SphMusic.h"
#include "SphHUD.h"
#include "SphGui.h"
#include "SphDesertTerrain.h"
#include "SphMovingProp.h"
#include "SphTeleportParticles.h"
#include "SphFireParticles.h"

SphMesh SphAvatar::s_meshNormal[N_PAIR], SphAvatar::s_meshSpeed[N_PAIR], SphAvatar::s_meshRubber[N_PAIR], SphAvatar::s_meshMetal[N_PAIR];

//Create SphAvatar.
SphAvatar::SphAvatar(){
	this->m_controller = new SphAvatarController(this);
	this->LoadMeshes();
	this->Load("Meshes/Sphere/", "Sphere.drk");
	this->SetClosedType(Normal);
	this->m_bound = new SphSphericBound(this);

	m_attachments[LaserGun] = new SphLaserGunAttachment();
	m_attachments[SprayGun] = new SphSprayGunAttachment();
	m_attachments[Legs] = new SphLegsAttachment();

	//make sure if there are any meshes associated with attachments, they are cleared
	for(int n = 0; n < AttachmentCount; n++){
		m_attachments[n]->RemoveAttachment(this);
	}

	//clear all keys
	SetN(this->m_keys, false, sizeof(m_keys));

	//clear all cfa types
	SetN(this->m_cfa, false, sizeof(m_cfa));

	//set cacti to 0
	this->m_cacti = 0.0f;

	this->m_moveForce = 30.0f;
	this->m_rotationFactor = 0.3f;
	this->m_health = 100;
	this->m_onGround = false;
	this->m_frictionFactor = 1.0f;
	this->m_teleParticles = new SphTeleportParticles("./Textures/Particles/", "teleport.tga", this->GetPosition());
	this->m_fireParticles = new SphFireParticles("./Textures/Particles/", "fire.tga", this->GetPosition());
	this->m_currentTime = 0.0f;
	this->m_timeSinceTeleport = 0.0f;
	this->m_timeSinceFire = 0.0f;
}

//Render SphAvatar.
void SphAvatar::Render(){

	if(this->m_timeSinceTeleport > this->m_currentTime){
		this->m_teleParticles->Render();
	}
	if(this->m_timeSinceFire > this->m_currentTime){
		this->m_fireParticles->Render();
	}
	SphModel::Render();
}

//return - true if first collision is closer to avatar.
bool SortCollisions(SphCollisionInfo first, SphCollisionInfo second){
	cfloat* avatarPos = g_game.m_avatar->GetPosition();
	return DistanceSq(avatarPos, first.m_collisionPoint) < DistanceSq(avatarPos, second.m_collisionPoint);
}

//Process collisions with avatar.
void SphAvatar::ProcessCollisions(){
	m_onGround = false;
	m_frictionFactor = 0.94f;//default air friction

	//check if below ground
	float pos[N_XYZ]; Set(pos, GetPosition());
	if(pos[O_X] > 0.0f && pos[O_Z] > 0.0f){
		float groundY = g_game.m_terrain->getHeightc(pos[O_X], pos[O_Z]);
		if(pos[O_Y] < groundY){
			pos[O_Y] = groundY + 0.9f;
			SetPosition(pos, false);
		}
	}

	//collide with normal objects
	SphCollisionsList collisionsList;
	g_game.m_world->GetCollisions(&collisionsList, m_bound);
	collisionsList.sort(SortCollisions);
	for(SphCollisionsList::iterator iter = collisionsList.begin(); iter != collisionsList.end(); iter++){
		ProcessCollision(&*iter);
	}

	//collide with items
	collisionsList.clear();
	g_game.m_world->GetItemCollisions(&collisionsList, m_bound);
	for(SphCollisionsList::iterator iter = collisionsList.begin(); iter != collisionsList.end(); iter++){
		((SphItem*)(*iter).m_object)->ItemPickup(this);
	}
}

//info - collision info to process.
void SphAvatar::ProcessCollision(SphCollisionInfo* info){
	//translate the object far enough from the collision point
	float fix[N_XYZ], diff[N_XYZ], magnitudeDiff;
	float radius = ((SphSphericBound*)m_bound)->m_radius;
	Subtract(diff, GetPosition(), info->m_collisionPoint);
	magnitudeDiff = Magnitude(diff);
	if(magnitudeDiff > radius) return;

	Multiply(diff, 1.0f / magnitudeDiff);
	float velocity[N_XYZ]; Set(velocity, GetVelocity());
	float velMagnitude = Magnitude(velocity);
	Multiply(velocity, 1.0f / velMagnitude);

	//if diff is approximately in the direction of velocity and we are going fast enough
	//we need to check if we went too far and the fix is going to be in the opposite direction
	if(Dot(diff, velocity) > 0.7f && Abs(velMagnitude) > 15.0f){
		Multiply(diff, -1.0f);
		Set(fix, diff);
		Multiply(fix, radius + magnitudeDiff);
	} else {
		Set(fix, diff);
		Multiply(fix, radius - magnitudeDiff);
	}
	
	Translate(fix);

	//compute bounce/friction factors
	float bounceFactor = 0.7f;
	if(diff[O_Y] > 0.5f){//if the normal points approximately up, we collided with some kind of ground.
		m_onGround = true;
		if(info->m_object->GetObjType() == SphObject::PROP_OBJ){
			m_frictionFactor = ((SphProp*)info->m_object)->GetFrictionFactor();
			bounceFactor = ((SphProp*)info->m_object)->GetBounceFactor();
		} else if(info->m_object->GetObjType() == SphObject::BASE_OBJ){ //ground is now "BASE_OBJ" type
			m_frictionFactor = 0.93f;//default ground values
			bounceFactor = 0.5f;
		} else if(info->m_object->GetObjType() == SphObject::MOVING_PROP_OBJ){
			m_frictionFactor = ((SphMovingProp*)info->m_object)->GetFrictionFactor();
			bounceFactor = ((SphMovingProp*)info->m_object)->GetBounceFactor();
			((SphMovingProp*)info->m_object)->Trigger();
		}
	}

	//adjust velocity based on collision normal
	float proj[N_XYZ];
	Set(velocity, GetVelocity());
	VectorProject(proj, velocity, diff);
	SubtractFrom(velocity, proj);
	Multiply(proj, -bounceFactor * GetBounceFactor());
	if(GetBounceFactor()> 0.9f && Magnitude(proj) > 5.0f){
		g_game.PlaySound("Audio//rubberBounce.ogg", 1.0f, false);
	}
	AddTo(velocity, proj);

	if(info->m_object->GetObjType() == SphObject::ENEMY_OBJ)
	{
		Add(velocity, velocity, info->m_object->GetVelocity());
		Multiply(velocity, 0.5f);
	}
	SetVelocity(velocity);
}

//Update SphAvatar.
void SphAvatar::Update(float seconds){
	this->m_currentTime += seconds;
	if(g_sdlInput.IsKeyDown(SDLK_q)) printf("%f %f %f\n", GetPosition()[0], GetPosition()[1], GetPosition()[2]);
	//check if player is still alive
	if(this->GetHealth() == 0){
		this->m_timeSinceFire = 0.0f;
		this->m_timeSinceTeleport = 0.0f;
		g_game.Reset();
	}

	m_controller->Update(seconds);
	float rotationFactor = IsClosed() ? m_rotationFactor : 0.0f;
	ApplyPhysics(seconds, m_frictionFactor, rotationFactor, !g_sdlInput.IsShiftDown());
	ProcessCollisions();

	//update attachments
	for(int n = 0; n < AttachmentCount; n++){
		m_attachments[n]->Update(seconds);
	}
	if(!IsClosed() && m_attachments[Legs]->IsAttached()){
		((SphSphericBound*)m_bound)->m_radius = 1.8f;
	} else {
		((SphSphericBound*)m_bound)->m_radius = 1.0f;
	}
	if(this->m_timeSinceTeleport > this->m_currentTime){
		this->m_teleParticles->ComputeParticles(g_game.m_avatar->GetPosition(), this->GetPosition());
	}

	if(this->m_timeSinceFire > this->m_currentTime){
		//Avatar only safe when closed + metal
		if(!(this->GetClosedType()==Metal) || !(this->IsClosed()))	
			this->TakeDamage(1);
		this->m_fireParticles->ComputeParticles(g_game.m_avatar->GetPosition(), this->GetPosition());
	}

	SphModel::Update(seconds);

}

//enable closed form ability for use
void SphAvatar::AddClosedType(SphAvatar::ClosedType type){
	if(!m_cfa[type]){
		m_cfa[type] = true;
		g_game.m_hud->EnableCFAButton(type);
	}
}

//disable closed form ability
void SphAvatar::RemoveClosedType(SphAvatar::ClosedType type){
	if(m_cfa[type]){
		m_cfa[type] = false;
		g_game.m_hud->DisableCFAButton(type);
	}
}

//type - avatar's closed type.
void SphAvatar::SetClosedType(ClosedType type){
	if(m_cfa[type]){
		#define SetMeshes(l, r) SetBoneMesh(3, &l); SetBoneMesh(6, &r);
		if(type != m_closedType){
			g_game.PlaySound("Audio/changeForm.ogg", 0.2f, false);
		}
		m_closedType = type;
		switch(type){
			case Normal: SetMeshes(s_meshNormal[0], s_meshNormal[1]); break;
			case Speed: SetMeshes(s_meshSpeed[0], s_meshSpeed[1]); break;
			case Rubber: SetMeshes(s_meshRubber[0], s_meshRubber[1]); break;
			case Metal: SetMeshes(s_meshMetal[0], s_meshMetal[1]); break;
		}
		#undef SetMeshes
	}
}

//return - the avatar's current closed-form type
SphAvatar::ClosedType SphAvatar::GetClosedType(){
	return m_closedType;
}

//return - avatar's current moving force.
float SphAvatar::GetMoveForce(){
	float modifier = 0.0f;
	if(IsClosed()){
		if(GetClosedType() == Speed) modifier = 1.5f;
		else if(GetClosedType() == Metal) modifier = 0.5f;
		else modifier = 1.0f;
	} else {
		if(m_attachments[Legs]->IsAttached()) modifier = 0.75f;
	}
	return m_moveForce * modifier;
}

//return - avatar's rotation factor.
float SphAvatar::GetRotationFactor(){
	return m_rotationFactor;
}

//return - multiply velocity by this factor when bouncing off an object.
float SphAvatar::GetBounceFactor(){
	if(IsClosed()){
		if(GetClosedType() == Rubber){ 
			return 1.0f;
		}
		if(GetClosedType() == Metal) return 0.1f;
		return 0.3f;
	}
	return 0.1f;
}

//return - true if avatar is closed.
bool SphAvatar::IsClosed(){
	return m_controller->IsClosed();
}

//return - true if avatar is on the ground.
bool SphAvatar::IsOnGround(){
	return m_onGround;
}

//Sets timer for teleportation particles
void SphAvatar::Teleporting(){
	this->m_timeSinceTeleport = this->m_currentTime + 1.0f;
}

void SphAvatar::SetOnFire(){
	g_game.PlaySound("Audio/fire.ogg", 1.0f, false);
	this->m_timeSinceFire = this->m_currentTime + 2.0f;
}

//Set hemisphere meshes.
void SphAvatar::LoadMeshes(){
	s_meshNormal[0].Load("Meshes/Sphere/", "HemisphereL.drkMesh");
	s_meshNormal[1].Load("Meshes/Sphere/", "HemisphereR.drkMesh");
	s_meshSpeed[0].Load("Meshes/Sphere/", "SpeedHemisphereL.drkMesh");
	s_meshSpeed[1].Load("Meshes/Sphere/", "SpeedHemisphereR.drkMesh");
	s_meshRubber[0].Load("Meshes/Sphere/", "RubberHemisphereL.drkMesh");
	s_meshRubber[1].Load("Meshes/Sphere/", "RubberHemisphereR.drkMesh");
	s_meshMetal[0].Load("Meshes/Sphere/", "MetalHemisphereL.drkMesh");
	s_meshMetal[1].Load("Meshes/Sphere/", "MetalHemisphereR.drkMesh");
}

//attachment - to add to the sphere.
void SphAvatar::AddAttachment(Attachment attachment){
	if(!m_attachments[attachment]->IsAttached()){
		m_attachments[attachment]->AddAttachment(this);
		g_game.m_hud->EnableOFAButton(attachment);
	}
}

//attachment - to remove from the sphere.
void SphAvatar::RemoveAttachment(Attachment attachment){
	if(m_attachments[attachment]->IsAttached()){
		m_attachments[attachment]->RemoveAttachment(this);
		g_game.m_hud->DisableOFAButton(attachment);
	}
}

//return true if avatar has attachment
bool SphAvatar::HasAttachment(Attachment attachment){
	return m_attachments[attachment]->IsAttached();
}

//return true if avatar has closed type
bool SphAvatar::HasClosedType(ClosedType type){
	return m_cfa[type];
}

//keys - add key to keyring
void SphAvatar::AddKey(SphKey::keyType color){
	if(!m_keys[color]){
		m_keys[color] = true;
		g_game.m_hud->EnableKeyButton(color);
	}
}

//keys - remove key from keyring
void SphAvatar::RemoveKey(SphKey::keyType color){
	if(m_keys[color]){
		m_keys[color] = false;
		g_game.m_hud->DisableKeyButton(color);
	}
}

//return true if avatar has specified key, false otherwise
bool SphAvatar::HasKey(SphKey::keyType color){
	return m_keys[color];
}

void SphAvatar::AddCactus(){
	g_game.m_hud->EnableCactusImage(m_cacti);
	m_cacti++;
	if (m_cacti == 7){
		g_game.m_hud->EnableAchievement(0);
	}
}

int SphAvatar::GetCacti(){
	return m_cacti;
}

//return this object's object type
SphObject::SphObjectType SphAvatar::GetObjType(){
	return AVATAR_OBJ;
}

void SphAvatar::Quit(){
	RemoveClosedType(SphAvatar::Normal);
	RemoveClosedType(SphAvatar::Speed);
	RemoveClosedType(SphAvatar::Rubber);
	RemoveClosedType(SphAvatar::Metal);

	RemoveAttachment(SphAvatar::LaserGun);
	RemoveAttachment(SphAvatar::SprayGun);
	RemoveAttachment(SphAvatar::Legs);

	RemoveKey(SphKey::Red);
}

void SphAvatar::Reset(){
	this->Quit();
	AddClosedType(SphAvatar::Normal);
	ChangeHealth(100);
	SetClosedType(SphAvatar::Normal);
}