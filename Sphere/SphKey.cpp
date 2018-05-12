#include "SphKey.h"
#include "SphMesh.h"
#include "SphAvatar.h"
#include "SphGame.h"
#include "SphOpenAL.h"
#include "SphWorld.h"
#include "SphSphericBound.h"

SphKey g_keys;

SphKey::SphKey() {
	this->m_bound = new SphSphericBound(this, 1.5f);//TODO: get correct radius from correct mesh
}

// Load in meshes to be used for Keys.
void SphKey::Init(){
	m_redKey.Load("Meshes/Items/", "RedGear.drkMesh");
	m_orangeKey.Load("Meshes/Items/", "OrangeGear.drkMesh");
	m_yellowKey.Load("Meshes/Items/", "YellowGear.drkMesh");
	m_greenKey.Load("Meshes/Items/", "GreenGear.drkMesh");
	m_blueKey.Load("Meshes/Items/", "BlueGear.drkMesh");
	m_indigoKey.Load("Meshes/Items/", "IndigoGear.drkMesh");
	m_violetKey.Load("Meshes/Items/", "VioletGear.drkMesh");
	}

// Sets all of the new Key prop's attributes.
void SphKey::CreateKey(SphKey* key, cchar* name) {
	if(!strcmp(name, "red")) {
		key->SetMesh(&m_redKey);
		key->m_key = Red;
	}
	else if(!strcmp(name, "orange")) {
		key->SetMesh(&m_orangeKey);
		key->m_key = Orange;
	}
	else if(!strcmp(name, "yellow")) {
		key->SetMesh(&m_yellowKey);
		key->m_key = Yellow;
	}
	else if(!strcmp(name, "green")) {
		key->SetMesh(&m_greenKey);
		key->m_key = Green;
	}
	else if(!strcmp(name, "blue")) {
		key->SetMesh(&m_blueKey);
		key->m_key = Blue;
	}
	else if(!strcmp(name, "indigo")) {
		key->SetMesh(&m_indigoKey);
		key->m_key = Indigo;
	}
	else if(!strcmp(name, "violet")) {
		key->SetMesh(&m_violetKey);
		key->m_key = Violet;
	}
	key->m_bound = new SphSphericBound(key, 1.25f);
}

//Handles a collision with an avatar object
void SphKey::ItemPickup(SphAvatar* avatar){
	avatar->AddKey(this->m_key);
	g_game.m_world->RemoveItem(this);
	g_game.PlaySound("Audio/pickUp.ogg", 0.75f, false);
}

//return - new prop based on the given name. NULL if incorrect name.
SphKey* SphKey::CreateNewProp(cchar* name){
	SphKey* key = new SphKey();
	CreateKey(key, name);
	return key;
}