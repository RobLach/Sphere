#include "SphMovingPropAgent.h"
#include "SphBasicAgentInterfaces.h"
#include "SphMovingProp.h"

/**
* Special constructor for class SphMovingPropAgent
*
* args: propObject - The moving prop object that this agent is controlling
*		startState - The state that the agent starts in
*/
SphMovingPropAgent::SphMovingPropAgent(SphMovingProp* propObject, SphBasicState *startState)
{
	this->m_object = propObject;
	this->m_currentState = startState;
}

/**
* CheckIsSafe
*
* Checks if the object should remain in its idle state
*
* returns: true if the object should remain idle
*/
bool SphMovingPropAgent::CheckStayIdle()
{
	return ((IBaseMovingPropAgent*)m_object)->CheckStayIdle();
}

/**
* CheckDestinationReached
*
* Checks if the object has reached its destination
*
* returns: true if the object has reached its destination
*/
bool SphMovingPropAgent::CheckDestinationReached()
{
	return ((IBaseMovingPropAgent*)m_object)->CheckDestinationReached();
}

/**
* IdleAction
*
* Executes the object's "Idle" state commands
*
* args: seconds - The seconds since the last update
*/
void SphMovingPropAgent::IdleAction(float seconds)
{
	((IBaseMovingPropAgent*)m_object)->IdleAction(seconds);
}

/**
* MoveAction
*
* Executes the object's "Move" state commands
*
* args: seconds - The seconds since the last update
*/
void SphMovingPropAgent::MoveAction(float seconds)
{
	((IBaseMovingPropAgent*)m_object)->MoveAction(seconds);
}