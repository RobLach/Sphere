#pragma once
#include "SphInclude.h"

class SphBound;
struct SphCollisionInfo;

//SphObject is a basic object in the game world.
class SphObject{

public:
	enum SphObjectType { BASE_OBJ, PROP_OBJ, AVATAR_OBJ, GROUND_OBJ, ENEMY_OBJ, ITEM_OBJ, PROJ_OBJ, MOVING_PROP_OBJ};

private:
	float m_position[N_XYZ];
	float m_rotation[N_QUAT];
	// Physics stuff.
	float m_netForce[N_XYZ];
	float m_velocity[N_XYZ];
	float m_physicsTime;

protected:
	int m_health;

	void ApplyMatrix();

public:
	SphBound* m_bound;

	SphObject();
	virtual void Render();
	virtual void Update(float seconds);
	void Translate(cfloat* vector, bool updateBound = true);
	void Rotate(cfloat* quaternion);
	void RotateWorldSpace(cfloat* worldSpaceQuaternion);
	void Translate3(float x, float y, float z, bool updateBound = true);
	void SetPosition(cfloat* position, bool updateBound = true);
	void SetPosition(float x, float y, float z, bool updateBound = true);
	void SetRotation(cfloat* quaternion);
	cfloat* GetPosition();
	cfloat* GetRotation();

	bool m_damagedByLaser;
	bool m_damagedBySpray;

	void SetBound(SphBound* newBound);

	void ChangeHealth(int delta);
	void SphObject::TakeDamage(int damage);
	int GetHealth();
	virtual SphObjectType GetObjType();

	//phyics functions
	void AddForce(cfloat* force);
	void ApplyPhysics(float seconds, float frictionFactor, float rotationFactor, bool gravity);

	cfloat* GetVelocity();
	cfloat* GetNetForce();
	
	void SetVelocity(cfloat* velocity);
	void SetNetForce(cfloat* netForce);
};
