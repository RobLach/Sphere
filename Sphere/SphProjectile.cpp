#include "SphMath.h"
#include "SphProjectile.h"
#include "SphPointBound.h"

//Create SphProjectile.
SphProjectile::SphProjectile(){
	this->m_damage = 25;
	this->m_bound = new SphPointBound(this);
	this->m_alive = true;
}

//Set initital parameters for SphProjectile.
void SphProjectile::Create(cfloat* position, cfloat* target){
	SetPosition(position);
	Set(m_target, target);
}

//Returns damage amount dealt by projectile.
int SphProjectile::GetDamage(){
	return m_damage;
}

//return - true if the projectile is still alive.
bool SphProjectile::IsAlive(){
	return m_alive;
}

//collision resolution
SphObject::SphObjectType SphProjectile::GetObjType(){
	return PROJ_OBJ;
}

