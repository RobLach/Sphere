#pragma once
#include "SphInclude.h"
#include "SphWorld.h"

class SphProp;

//SphDesertWorld is the world for the desert level.
class SphDesertWorld : public SphWorld {

protected:
	SphProp* m_yellowDoor;

	void AddPuzzles();
	void AddItems();
	void AddEnemies();
	void AddOther();
	void AddLights();

public:
	SphDesertWorld();
	void Update(float seconds);

};