#pragma once
#include "SphInclude.h"
#include "SphObject.h"

//SphObjectController controls an object according to user input.
class SphEnemyController{

public:
	SphObject* m_object;

	SphEnemyController();
	void Update(float seconds);
};
