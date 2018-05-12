#include "SphAbilityItem.h"
#include "SphMesh.h"
#include "SphAvatar.h"
#include "SphGame.h"
#include "SphWorld.h"
#include "SphSphericBound.h"


#include "SphMath.h"

SphAbilityItem g_abilityItems;

float bobValue = 0.0f;

// Load meshes to be used for SphAbilityItem.
void SphAbilityItem::Init(){
	m_legs.Load("Meshes/Sphere/", "LowerLegR.drkMesh");
	m_laser.Load("Meshes/Sphere/", "LaserGun.drkMesh");
	m_spray.Load("Meshes/Sphere/", "SprayGun.drkMesh");
	m_rubber.Load("Meshes/Items/", "Eraser.drkMesh");
	m_speed.Load("Meshes/Items/", "Speed.drkMesh");
	m_metal.Load("Meshes/Items/", "Magnet.drkMesh");
}

// Create SphAbilityItem
SphAbilityItem::SphAbilityItem() {
}

//Create SphAbilityItem.
void SphAbilityItem::CreateAbilityItem(SphAbilityItem* item, cchar* name){
	if(!strcmp(name, "laser")) 
	{
		item->SetMesh(&m_laser);
		item->SetBound(new SphSphericBound(item, 1.25f));//TODO: get correct radius from correct mesh
		item->m_abilityItemType = Laser;
	}
	else if(!strcmp(name, "spray")) 
	{
		item->SetMesh(&m_spray);
		item->m_bound = new SphSphericBound(item, 1.25f);//TODO: get correct radius from correct mesh
		item->m_abilityItemType = Spray;
	}
	else if(!strcmp(name, "legs")) 
	{
		item->SetMesh(&m_legs);
		item->m_bound = new SphSphericBound(item, 1.25f);//TODO: get correct radius from correct mesh
		item->m_abilityItemType = Legs;
	}
	else if(!strcmp(name, "rubber")) 
	{
		item->SetMesh(&m_rubber);
		item->m_bound = new SphSphericBound(item, 1.25f);//TODO: get correct radius from correct mesh
		item->m_abilityItemType = Rubber;
	}
	else if(!strcmp(name, "metal")) 
	{
		item->SetMesh(&m_metal);
		item->m_bound = new SphSphericBound(item, 1.25f);//TODO: get correct radius from correct mesh
		item->m_abilityItemType = Metal;
	}
	else if(!strcmp(name, "speed")) 
	{
		item->SetMesh(&m_speed);
		item->m_bound = new SphSphericBound(item, 1.25f);//TODO: get correct radius from correct mesh
		item->m_abilityItemType = Speed;
	}
	
	item->m_bobAmplitude = 0.01f;
	item->m_bobFrequency = 0.25f;
}

//Handles a collision with an avatar object
void SphAbilityItem::ItemPickup(SphAvatar* avatar){
	//avatar->ChangeHealth(m_healthValue);
	//this->m_type

	switch(this->m_abilityItemType) {
		case Speed: avatar->AddClosedType(SphAvatar::Speed); break;
		case Rubber: avatar->AddClosedType(SphAvatar::Rubber); break;
		case Metal: avatar->AddClosedType(SphAvatar::Metal); break;
		case Laser: avatar->AddAttachment(SphAvatar::LaserGun); break;
		case Spray: avatar->AddAttachment(SphAvatar::SprayGun); break;
		case Legs: avatar->AddAttachment(SphAvatar::Legs); break;
	}

	g_game.m_world->RemoveItem(this);
	g_game.PlaySound("Audio/powerUp.ogg", 0.75f, false);

}

//Update SphObject.
//seconds - since the last call.
void SphAbilityItem::Update(float seconds){
	//yay rotation!
	float rotate[N_QUAT], position[N_XYZ];
	QuaternionY(rotate, (0.5f * seconds));
	this->Rotate(rotate);

	//yay bobbing!
	bobValue += seconds * m_bobFrequency;			//set frequency of bobbing... 0.25?
	Set(position, this->GetPosition());
	Set(position,	position[O_X],
					position[O_Y] + m_bobAmplitude*Sin(bobValue),	//bobHeight is just amplitude..
					position[O_Z]);
	this->SetPosition(position);
	//printf("%f %f %f %f\n", rotate[O_QW], rotate[O_QX], rotate[O_QY], rotate[O_QZ]);

	SphItem::Update(seconds);
}

//return - new prop based on the given name. NULL if incorrect name.
SphAbilityItem* SphAbilityItem::CreateNewProp(cchar* name){
	SphAbilityItem* item = new SphAbilityItem();
	CreateAbilityItem(item, name);
	return item;
}

//Sets the amplitude of the bobbing
void SphAbilityItem::SetBobAmplitude(cfloat bobAmplitude){
	this->m_bobAmplitude = bobAmplitude;
}

//Sets the frequency of the bobbing
void SphAbilityItem::SetBobFrequency(cfloat bobFrequency){
	this->m_bobFrequency = bobFrequency;
}