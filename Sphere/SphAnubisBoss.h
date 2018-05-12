#pragma once
#include "SphInclude.h"
#include "SphBasicAgentInterfaces.h"
#include "SphEnemyBoss.h"

//SphAnubiEnemy is a SphEnemyBoss, who trolls 4chan's /b/ board
class SphAnubisBoss : public SphEnemyBoss {

protected:
	//pre-attack state delay variables
	float m_timeInPreAttackState;
	float m_secondsOfPreAttackDelay;
	//time between when an attack ends and a new pre-attack->attack starts
	float m_timeSinceLastAttack;
	float m_secondsBetweenAttacks;

public:
	SphAnubisBoss();
	
	void Update(float seconds);

	//BOOLEAN CHECKS
		//IBaseEnemyAgent Interface
	bool CheckIsSafe();
	bool CheckDestinationReached();
		//IBaseEnemyBossAgent Interface
	bool CheckDoPreAttack();
	bool CheckDoAttack();

	//STATE ACTIONS
		//IBaseEnemyAgent Interface
	void HostileAction(float seconds);
	void IdleAction(float seconds);
	void MoveAction(float seconds);
		//IBaseEnemyBossAgent Interface
	void PreAttackAction(float seconds);
	
	//AI-related functions
};
