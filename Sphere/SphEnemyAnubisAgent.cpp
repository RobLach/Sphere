#include "SphEnemyAnubisAgent.h"
#include "SphBasicAgentInterfaces.h"
#include "SphEnemyBoss.h"

/**
* Special constructor for class SphEnemyCubeAgent
*
* args: cubeObject - The cube enemy object that this agent is controlling
*		startState - The state that the agent starts in
*/
SphEnemyAnubisAgent::SphEnemyAnubisAgent(SphEnemyBoss* anubisObject, SphBasicState *startState)
{
	this->m_object = anubisObject;
	this->m_currentState = startState;
}

/**
* CheckIsSafe
*
* Checks if the object is currently safe
*
* returns: true if the object is "safe"
*/
bool SphEnemyAnubisAgent::CheckIsSafe()
{
	return ((IBaseEnemyAgent*)m_object)->CheckIsSafe();
}

/**
* CheckRunAway
*
* Checks if the object should flee from its current position
*
* returns: true if the object should "run away"
*/
bool SphEnemyAnubisAgent::CheckRunAway()
{
	return ((IBaseEnemyAgent*)m_object)->CheckRunAway();
}

/**
* CheckDestinationReached
*
* Checks if the object has reached its destination
*
* returns: true if the object has reached its destination
*/
bool SphEnemyAnubisAgent::CheckDestinationReached()
{
	return ((IBaseEnemyAgent*)m_object)->CheckDestinationReached();
}

/**
* CheckDoPreAttack
*
* Checks if the object should start doing its pre-attack action
*
* returns: true if the object should start doing its pre-attack action
*/
bool SphEnemyAnubisAgent::CheckDoPreAttack()
{
	return ((IBaseEnemyBossAgent*)m_object)->CheckDoPreAttack();
}

/**
* CheckDoAttack
*
* Checks if the object should start doing its attack action
*
* returns: true if the object should start doing its attack action
*/
bool SphEnemyAnubisAgent::CheckDoAttack()
{
	return ((IBaseEnemyBossAgent*)m_object)->CheckDoAttack();
}


/**
* HostileAction
*
* Executes the object's "Hostile" state commands
*
* args: seconds - The seconds since the last update
*/
void SphEnemyAnubisAgent::HostileAction(float seconds)
{
	((IBaseEnemyAgent*)m_object)->HostileAction(seconds);
}

/**
* AvoidAction
*
* Executes the object's "Avoid" state commands
*
* args: seconds - The seconds since the last update
*/
void SphEnemyAnubisAgent::AvoidAction(float seconds)
{
	((IBaseEnemyAgent*)m_object)->AvoidAction(seconds);
}

/**
* IdleAction
*
* Executes the object's "Idle" state commands
*
* args: seconds - The seconds since the last update
*/
void SphEnemyAnubisAgent::IdleAction(float seconds)
{
	((IBaseEnemyAgent*)m_object)->IdleAction(seconds);
}

/**
* MoveAction
*
* Executes the object's "Move" state commands
*
* args: seconds - The seconds since the last update
*/
void SphEnemyAnubisAgent::MoveAction(float seconds)
{
	((IBaseEnemyAgent*)m_object)->MoveAction(seconds);
}


/**
* PreAttackAction
*
* Executes the object's "Pre-Attack" state commands
*
* args: seconds - The seconds since the last update
*/
void SphEnemyAnubisAgent::PreAttackAction(float seconds)
{
	((IBaseEnemyBossAgent*)m_object)->PreAttackAction(seconds);
}
