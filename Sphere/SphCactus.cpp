#include "SphCactus.h"
#include "SphMesh.h"
#include "SphAvatar.h"
#include "SphGame.h"
#include "SphOpenAL.h"
#include "SphWorld.h"
#include "SphSphericBound.h"

SphCactus g_cacti;

SphCactus::SphCactus() {
}

// Load in meshes to be used for Keys.
void SphCactus::Init(){
	m_cactus.Load("Meshes/Props/", "Cactus.drkMesh");
	}

// Sets all of the new Key prop's attributes.
void SphCactus::CreateCactus(SphCactus* cactus, cchar* name) {
	cactus->SetMesh(&m_cactus);
	cactus->m_bound = new SphSphericBound(cactus, 1.25f);
}

//Handles a collision with an avatar object
void SphCactus::ItemPickup(SphAvatar* avatar){
	avatar->AddCactus();
	g_game.m_world->RemoveItem(this);
	g_game.PlaySound("Audio/pickUp.ogg", 0.75f, false);
}

//return - new prop based on the given name. NULL if incorrect name.
SphCactus* SphCactus::CreateNewProp(cchar* name){
	SphCactus* cactus = new SphCactus();
	CreateCactus(cactus, name);
	return cactus;
}