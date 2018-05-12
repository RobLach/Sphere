#include "SphMath.h"
#include "SphSdlInput.h"
#include "SphLaserGunAttachment.h"
#include "SphGame.h"
#include "SphWorld.h"
#include "SphAvatar.h"
#include "SphLaser.h"
#include "SphMesh.h"

SphMesh* SphLaserGunAttachment::s_laserGunMesh = NULL;

//Create SphLaserGunAttachment.
SphLaserGunAttachment::SphLaserGunAttachment(){
	if(s_laserGunMesh == NULL){
		s_laserGunMesh = new SphMesh();
		s_laserGunMesh->Load("Meshes/Sphere/", "LaserGun.drkMesh");
	}
}

//Update SphLaserGunAttachment.
//seconds - since last call.
void SphLaserGunAttachment::Update(float seconds){
	if(!m_attached) return;
	SphAvatar* avatar = g_game.m_avatar;
	if((g_sdlInput.KeyUpEvent(SDLK_LCTRL) || g_sdlInput.MouseButtonDownEvent(g_sdlInput.s_leftMouseButton)) && !avatar->IsClosed()){
		//calculate target (TODO: find enemy)
		float target[N_XYZ], start[N_XYZ];
		Add(start, avatar->GetPosition(), avatar->GetBoneEnd(7));
		Set(target, 0.0f, 0.0f, 10000.0f);//default forward direction
		QuatApplyTo(target, target, avatar->GetRotation());
		AddTo(target, start);

		//spawn laser
		SphLaser* laser = new SphLaser();
		laser->Create(start, target);
		laser->SetRotation(avatar->GetRotation());
		g_game.m_world->AddNonCollidable(laser);
	}
}

//Add this attachment to the avatar.
void SphLaserGunAttachment::AddAttachment(SphAvatar* avatar){
	avatar->SetBoneMesh(7, s_laserGunMesh);
	SphAttachment::AddAttachment(avatar);
}

//Remove this attachment from the avatar.
void SphLaserGunAttachment::RemoveAttachment(SphAvatar* avatar){
	avatar->SetBoneMesh(7, NULL);
	SphAttachment::RemoveAttachment(avatar);
}