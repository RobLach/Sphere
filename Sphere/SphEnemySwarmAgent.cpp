#include "SphEnemySwarmAgent.h"
#include "SphBasicAgentInterfaces.h"
#include "SphEnemy.h"

/**
* Special constructor for class SphEnemySwarmAgent
*
* args: swarmObject - The swarm enemy object that this agent is controlling
*		startState - The state that the agent starts in
*/
SphEnemySwarmAgent::SphEnemySwarmAgent(SphEnemy* swarmObject, SphBasicState *startState)
{
	this->m_object = swarmObject;
	this->m_currentState = startState;
}

/**
* CheckIsSafe
*
* Checks if the object is currently safe
*
* returns: true if the object is "safe"
*/
bool SphEnemySwarmAgent::CheckIsSafe()
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
bool SphEnemySwarmAgent::CheckRunAway()
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
bool SphEnemySwarmAgent::CheckDestinationReached()
{
	return ((IBaseEnemyAgent*)m_object)->CheckDestinationReached();
}

/**
* HostileAction
*
* Executes the object's "Hostile" state commands
*
* args: seconds - The seconds since the last update
*/
void SphEnemySwarmAgent::HostileAction(float seconds)
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
void SphEnemySwarmAgent::AvoidAction(float seconds)
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
void SphEnemySwarmAgent::IdleAction(float seconds)
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
void SphEnemySwarmAgent::MoveAction(float seconds)
{
	((IBaseEnemyAgent*)m_object)->MoveAction(seconds);
}
