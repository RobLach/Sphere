#include "SphInclude.h"
#include "SphMovingPropStates.h"
#include "SphMovingPropAgent.h"

/**
* Execute (SphMovingPropIdleState class)
*
* Executes the "Idle State" action (handles state transition conditions and
*	conditions of execution of current state)
*
* args: agent - The agent controlling the object
*		seconds - The seconds since the last update
*/
void SphMovingPropIdleState::Execute(SphBasicAgent* agent, float seconds) {
	if(!((SphMovingPropAgent*)agent)->CheckStayIdle())
	{
		//change to move state if we should no longer be idle
		agent->ChangeState(new SphMovingPropMoveState());
	}
	else
	{
		((SphMovingPropAgent*)agent)->IdleAction(seconds);
	}
}

/**
* Execute (SphMovingPropMoveState class)
*
* Executes the "Move State" action (handles state transition conditions and
*	conditions of execution of current state)
*
* args: agent - The agent controlling the object
*		seconds - The seconds since the last update
*/
void SphMovingPropMoveState::Execute(SphBasicAgent* agent, float seconds) {
	if(((SphMovingPropAgent*)agent)->CheckDestinationReached())
	{
		//change to idle state if we've reached our destination
		agent->ChangeState(new SphMovingPropIdleState());
	}
	else
	{
		((SphMovingPropAgent*)agent)->MoveAction(seconds);
	}
}
