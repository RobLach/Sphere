#pragma once
#include "SphInclude.h"
#include "SphBasicAgentInterfaces.h"
#include "SphEnemy.h"

//SphEnemyBoss is derived from the SphEnemy class, but adds the IBaseEnemyBossAgent interface
class SphEnemyBoss : public SphEnemy, public IBaseEnemyBossAgent {

public:
	SphEnemyBoss();
	
	virtual void Update(float seconds);

	/////IBaseEnemyBossAgent Interface
	//Added boolean checks:
	virtual bool CheckDoPreAttack();
	virtual bool CheckDoAttack();
	//Added state actions:
	virtual void PreAttackAction(float seconds);
	//(AI-related functions)
};
