#pragma once
#include <list>
#include "SphInclude.h"
using namespace std;

class SphObject;
class SphBound;
class SphPointBound;
class SphSphericBound;
class SphTightBound;

//SphCollisionInfo hold computed information about the collision.
struct SphCollisionInfo {
	SphObject* m_object;
	float m_collisionPoint[N_XYZ];//point of collision
	float m_temp[N_XYZ];//temporary storage
	bool m_collision;//true if collision occured

	SphCollisionInfo(){ m_object = NULL; m_collision = false; }
};

typedef list<SphCollisionInfo> SphCollisionsList;

//Compute collision between two objects.
//info - structure to be filled out with collision information.
//o1 - object for which to check collision.
//o2 - object to check against.
extern void Collide(SphCollisionsList* collisionsList, SphBound* o1, SphBound* o2);

//Compute collision between a point and a spherical bounds.
extern void Collide(SphCollisionsList* collisionsList, SphPointBound* o1, SphSphericBound* o2);

//Compute collision between a point and a tight bounds.
extern void Collide(SphCollisionsList* collisionsList, SphPointBound* o1, SphTightBound* o2);

//Compute collision between two spheres.
extern void Collide(SphCollisionsList* collisionsList, SphSphericBound* o1, SphSphericBound* o2);

//Compute collision between a spheric and a tight bounds.
extern void Collide(SphCollisionsList* collisionsList, SphSphericBound* o1, SphTightBound* o2);