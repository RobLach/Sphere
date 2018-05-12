#include "SphMath.h"
#include "SphSdlInput.h"
#include "SphSprayGunAttachment.h"
#include "SphGame.h"
#include "SphWorld.h"
#include "SphAvatar.h"
#include "SphLaser.h"
#include "SphMesh.h"
#include "SphSpray.h"
#include <time.h>

SphMesh* SphSprayGunAttachment::s_sprayGunMesh = NULL;

//Create SphSprayGunAttachment.
SphSprayGunAttachment::SphSprayGunAttachment(){
	if(s_sprayGunMesh == NULL){
		s_sprayGunMesh = new SphMesh();
		s_sprayGunMesh->Load("Meshes/Sphere/", "SprayGun.drkMesh");
	}
}

//Update SphSprayGunAttachment.
//seconds - since last call.
void SphSprayGunAttachment::Update(float seconds){
	if(!m_attached) return;
	SphAvatar* avatar = g_game.m_avatar;
	if((g_sdlInput.KeyUpEvent(SDLK_LCTRL) || g_sdlInput.MouseButtonDownEvent(g_sdlInput.s_leftMouseButton)) && !avatar->IsClosed()){
		//calculate target (TODO: find enemy)
		float target[N_XYZ], start[N_XYZ], newTarget[N_XYZ], rotq[N_QUAT], rotA[N_QUAT], rotY;
		Add(start, avatar->GetPosition(), avatar->GetBoneEnd(7));
		Set(target, 0.0f, 0.0f, 10000.0f);//default forward direction
		cfloat * rots = avatar->GetRotation();  

		SphSpray* spray;

		// Temporary spray implementation until particles are made
		for(int x = 0; x < m_sprayCount; x ++) 
		{
			// Get y-axis rotation increment for this piece of the spray
			rotY = ((float)x *((float)((PI / (float)12.0) / m_sprayCount) )) - (PI / (float)36.0);

			// Reset our quaternion with the avatar's rotation
			rotq[0] = rots[0];
			rotq[1] = rots[1];
			rotq[2] = rots[2];
			rotq[3] = rots[3];

			// Determine this piece's destination
			QuaternionY(rotA,rotY);
			QuatMultiplyBy(rotq,rotA);
			QuatApplyTo(newTarget, target, rotq);
			AddTo(newTarget, start);

			//spawn spray
			spray = new SphSpray();
			spray->Create(start, newTarget);
			spray->SetRotation(rotq);
			g_game.m_world->AddNonCollidable(spray);
		}
	}
}

//Add this attachment to the avatar.
void SphSprayGunAttachment::AddAttachment(SphAvatar* avatar){
	avatar->SetBoneMesh(7, s_sprayGunMesh);
	SphAttachment::AddAttachment(avatar);
}

//Remove this attachment from the avatar.
void SphSprayGunAttachment::RemoveAttachment(SphAvatar* avatar){
	avatar->SetBoneMesh(7, NULL);
	SphAttachment::RemoveAttachment(avatar);
}