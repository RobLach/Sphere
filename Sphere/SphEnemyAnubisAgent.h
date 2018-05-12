#pragma once
#include "SphInclude.h"
#include "SphBasicAgent.h"
#include "SphBasicAgentInterfaces.h"

class SphEnemyBoss;


class SphEnemyAnubisAgent : public SphBasicAgent , public IBaseEnemyAgent {
//The purpose of this is for it to implement whatever interfaces the enemy implements.
//	(all enemies use the same interface right now, so code looks similar for all agent classes)

private:
	//this needs to be a SphEnemy type (can't cast SphObject to "interface")
	SphEnemyBoss* m_object;
public:
	SphEnemyAnubisAgent(SphEnemyBoss* anubisObject, SphBasicState* startState);
	//boolean self-checks
		//IBaseEnemyAgent
	virtual bool CheckIsSafe();
	virtual bool CheckRunAway();
	virtual bool CheckDestinationReached();
		//IBaseEnemyBossAgent
	virtual bool CheckDoPreAttack();
	virtual bool CheckDoAttack();

	//current state types
		//IBaseEnemyBossAgent
	virtual void HostileAction(float seconds);
	virtual void AvoidAction(float seconds);
	virtual void IdleAction(float seconds);
	virtual void MoveAction(float seconds);
		//IBaseEnemyBossAgent
	virtual void PreAttackAction(float seconds);
};