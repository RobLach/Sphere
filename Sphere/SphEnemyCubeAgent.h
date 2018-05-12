#pragma once
#include "SphInclude.h"
#include "SphBasicAgent.h"
#include "SphBasicAgentInterfaces.h"

class SphEnemy;

class SphEnemyCubeAgent : public SphBasicAgent , public IBaseEnemyAgent {

private:
	//this needs to be a SphEnemy type (can't cast SphObject to "interface")
	SphEnemy* m_object;
public:
	SphEnemyCubeAgent(SphEnemy* cubeObject, SphBasicState* startState);
	//boolean self-checks
	virtual bool CheckIsSafe();
	virtual bool CheckRunAway();
	virtual bool CheckDestinationReached();
	//current state types
	virtual void HostileAction(float seconds);
	virtual void AvoidAction(float seconds);
	virtual void IdleAction(float seconds);
	virtual void MoveAction(float seconds);
};