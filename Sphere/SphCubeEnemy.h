#pragma once
#include "SphInclude.h"
#include "SphEnemy.h"

//SphCubeEnemy is a SphEnemy, which looks like a cube.
class SphCubeEnemy : public SphEnemy {

protected:



public:
	SphCubeEnemy();
	
	void Update(float seconds);
	//IBaseAgent Interface
	bool CheckIsSafe();
	bool CheckRunAway();
	bool CheckDestinationReached();
	void HostileAction(float seconds);
	void AvoidAction(float seconds);
	void IdleAction(float seconds);
	void MoveAction(float seconds);
	//AI-related functions
	
};
