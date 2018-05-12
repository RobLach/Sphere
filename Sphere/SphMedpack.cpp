#include "SphMedpack.h"
#include "SphMesh.h"
#include "SphAvatar.h"
#include "SphOpenAL.h"
#include "SphGame.h"
#include "SphWorld.h"
#include "SphSphericBound.h"

SphMedpack g_medpacks;

// Load meshes to be used for Medpacks.
void SphMedpack::Init(){
	m_small.Load("Meshes/Items/", "SmallMedpack.drkMesh");
	m_medium.Load("Meshes/Items/", "MediumMedpack.drkMesh");
	m_full.Load("Meshes/Items/", "LargeMedpack.drkMesh");
}

// Create Medpack
SphMedpack::SphMedpack() {
}

//Create SphMedpack.
void SphMedpack::CreateMedpack(SphMedpack* pack, cchar* name){
	if(!strcmp(name, "small")) 
	{
		pack->SetMesh(&m_small);
		pack->m_healthValue = 30;
		pack->SetBound(new SphSphericBound(pack, 1.25f));//TODO: get correct radius from correct mesh
	}
	else if(!strcmp(name, "medium")) 
	{
		pack->SetMesh(&m_medium);
		pack->m_healthValue = 60;
		pack->m_bound = new SphSphericBound(pack, 1.25f);//TODO: get correct radius from correct mesh
	}
	else if(!strcmp(name, "full")) 
	{
		pack->SetMesh(&m_full);
		pack->m_healthValue = 100;
		pack->m_bound = new SphSphericBound(pack, 1.25f);//TODO: get correct radius from correct mesh
	}
}

//Handles a collision with an avatar object
void SphMedpack::ItemPickup(SphAvatar* avatar){
	avatar->ChangeHealth(m_healthValue);
	g_game.m_world->RemoveItem(this);
	g_game.PlaySound("Audio/pickUp.ogg", 0.75f, false);
}

//return - new prop based on the given name. NULL if incorrect name.
SphMedpack* SphMedpack::CreateNewProp(cchar* name){
	SphMedpack* pack = new SphMedpack();
	CreateMedpack(pack, name);
	return pack;
}