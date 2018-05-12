#include "SphMath.h"
#include "SphCrate.h"
#include "SphTightBound.h"
#include "SphGame.h"
#include "SphAvatar.h"

//Crate SphCrate.
SphCrate::SphCrate(){
	Load("Meshes/Props/", "Crate.drk");
	m_bound = new SphTightBound(this);
	((SphTightBound*)m_bound)->Load("Meshes/Props/Crate.bound.drkMesh");
}

//Update SphCrate.
void SphCrate::Update(float seconds){
	SphAvatar* avatar = g_game.m_avatar;
	if(DistanceSq(GetPosition(), avatar->GetPosition()) < 4.5f){
		if(avatar->HasKey(SphKey::Red) && m_bound != NULL){
			DELETE(m_bound);
			PlayAction(1, 0.0f, false, true);
		}
	}
	SphModel::Update(seconds);
}