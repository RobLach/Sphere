#pragma once
#include "SphInclude.h"
#include "SphModel.h"
#include "SphBasicAgentInterfaces.h"
#include "SphProjectile.h"

struct SphCollisionInfo;
class SphBasicAgent;

//Enemy Class, Represents Enemy Object In-Game
class SphEnemy : virtual public SphModel, public IBaseEnemyAgent {

public:
	SphBasicAgent* m_agent;

protected:
	bool m_flying;

	float m_timeSinceLastAction;
	float m_secondsBetweenActions;
	bool m_newAttack;

	void ProcessCollisions();
	void ProcessCollision(SphCollisionInfo* info);

public:
	SphEnemy();
	void Render();
	void Update(float seconds);
	void TakeDamage(int damage);
	void Attack(SphObject* target);
	SphObject::SphObjectType GetObjType();

	int m_damage;
	int GetDamage();

	
	//IBaseEnemyAgent Interface
	virtual bool CheckIsSafe();
	virtual bool CheckRunAway();
	virtual bool CheckDestinationReached();
	virtual void HostileAction(float seconds);
	virtual void AvoidAction(float seconds);
	virtual void IdleAction(float seconds);
	virtual void MoveAction(float seconds);
	//AI-related functions
};
