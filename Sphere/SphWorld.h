#pragma once
#include <list>
#include "SphInclude.h"
#include "SphCollisionDetection.h"
#include "SphExplosionParticles.h"
using namespace std;

class SphObject;
class SphItem;
typedef list<SphObject*> SphObjectList;

//SphWorld contains SphGroups and provides useful functions.
class SphWorld {

protected:
	SphObjectList m_collidables;
	SphObjectList m_nonCollidables;
	SphObjectList m_items;

	float m_startPos[N_XYZ];
	float m_explosionPos[N_XYZ];

	void GetCollisions(SphCollisionsList* collisionsList, SphBound* bound, SphObjectList* objectList);

public:
	SphWorld();
	virtual void Render();
	virtual void Update(float seconds);

	virtual void AddCollidable(SphObject* object);
	virtual void RemoveCollidable(SphObject* object);
	virtual void AddNonCollidable(SphObject* object);
	virtual void RemoveNonCollidable(SphObject* object);
	virtual void AddItem(SphObject* item);
	virtual void RemoveItem(SphObject* item);

	cfloat* GetSpawnPosition();

	void GetCollisions(SphCollisionsList* collisionsList, SphBound* bound);
	void GetItemCollisions(SphCollisionsList* collisionsList, SphBound* bound);
	void GenerateExplosion(cfloat* position);
	SphExplosionParticles* m_explosion;


	virtual void Free();
};
