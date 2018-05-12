#pragma once
#include "SphInclude.h"
#include "SphEnemy.h"

class SphMehs;

//SphSwarmEnemy is a SphEnemy, which looks like a swarm of bees.
class SphSwarmEnemy : public SphEnemy {

protected:
	static SphMesh* s_beeMesh;
	static const int s_beeCount = 30;

	float m_beesPos[s_beeCount * N_XYZ];
	float m_time;



public:
	SphSwarmEnemy();
	void Render();
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
