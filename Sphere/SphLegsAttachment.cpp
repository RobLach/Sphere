#include "SphMath.h"
#include "SphSdlInput.h"
#include "SphLegsAttachment.h"
#include "SphGame.h"
#include "SphWorld.h"
#include "SphAvatar.h"
#include "SphLaser.h"
#include "SphMesh.h"

SphMesh* SphLegsAttachment::s_upperLegs[N_PAIR] = {NULL, NULL};
SphMesh* SphLegsAttachment::s_lowerLegs[N_PAIR] = {NULL, NULL};

//Create SphLegsAttachment.
SphLegsAttachment::SphLegsAttachment(){
	if(s_upperLegs[0] == NULL){
		s_upperLegs[0] = new SphMesh("Meshes/Sphere/", "UpperLegL.drkMesh");
		s_upperLegs[1] = new SphMesh("Meshes/Sphere/", "UpperLegR.drkMesh");
		s_lowerLegs[0] = new SphMesh("Meshes/Sphere/", "LowerLegL.drkMesh");
		s_lowerLegs[1] = new SphMesh("Meshes/Sphere/", "LowerLegR.drkMesh");
	}
}

//Update SphLaserGunAttachment.
//seconds - since last call.
void SphLegsAttachment::Update(float seconds){
	if(!m_attached) return;
	SphAvatar* avatar = g_game.m_avatar;
	if(g_sdlInput.IsKeyDown(SDLK_SPACE) && !avatar->IsClosed() && avatar->IsOnGround()){
		float jumpForce[N_XYZ];
		Set(jumpForce, 0.0f, 300.f, 0.0f);
		avatar->AddForce(jumpForce);
	}
}

//Add this attachment to the avatar.
void SphLegsAttachment::AddAttachment(SphAvatar* avatar){
	avatar->SetBoneMesh(13, s_upperLegs[0]); avatar->SetBoneMesh(9, s_upperLegs[1]);
	avatar->SetBoneMesh(15, s_lowerLegs[0]); avatar->SetBoneMesh(11, s_lowerLegs[1]);
	SphAttachment::AddAttachment(avatar);
}

//Remove this attachment from the avatar.
void SphLegsAttachment::RemoveAttachment(SphAvatar* avatar){
	avatar->SetBoneMesh(13, NULL); avatar->SetBoneMesh(9, NULL);
	avatar->SetBoneMesh(15, NULL); avatar->SetBoneMesh(11, NULL);
	SphAttachment::RemoveAttachment(avatar);
}