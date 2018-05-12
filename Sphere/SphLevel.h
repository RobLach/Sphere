#pragma once
#include <list>
#include "SphWorld.h"
//#include "SphTerrain.h"
//#include "SphEntity.h"
#include "SphInclude.h"
using namespace std;


class SphLevel: public SphWorld {
private:
	SphObjectList m_objects;
	//SphTerrain m_terrain;
	//SphEntityList m_entities;

	cfloat* SpawnPosition[N_XYZ];
	cfloat* GoalPosition[N_XYZ];

protected:

	void LoadLevel();

public:

	SphLevel();
	void Render();
	void Update(float seconds);

	void AddObject(SphObject* object);
	void RemoveObject(SphObject* object);

	void Free();

};