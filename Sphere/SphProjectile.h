#pragma once
#include "SphInclude.h"
#include "SphProp.h"

//SphProjectile is SphObject, which represents is a projectile.
class SphProjectile : public SphProp {

public:
	enum ShotType { Laser, Spray, Missile };

protected:
	float m_target[N_XYZ];
	int m_damage;
	ShotType m_shotType;
	int m_lifetime;
	float m_speed;
	int m_range;
	bool m_alive;

public:
	SphProjectile();
	void Create(cfloat* position, cfloat* target);
	int GetDamage();
	bool IsAlive();

	//collision resolution
	virtual SphObject::SphObjectType GetObjType();
};
