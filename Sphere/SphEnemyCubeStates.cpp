#include "SphInclude.h"
#include "SphEnemyCubeStates.h"
#include "SphEnemyCubeAgent.h"

/**
* Execute (SphEnemyCubeHostileState class)
*
* Executes the "Hostile State" action (handles state transition conditions and
*	conditions of execution of current state)
*
* args: agent - The agent controlling the object
*		seconds - The seconds since the last update
*/
void SphEnemyCubeHostileState::Execute(SphBasicAgent* agent, float seconds) {
	if(((SphEnemyCubeAgent*)agent)->CheckRunAway())
	{
		//change to flee state if you need to run away
		agent->ChangeState(new SphEnemyCubeAvoidState());
	}
	else if(((SphEnemyCubeAgent*)agent)->CheckIsSafe())
	{
		//change to idle state if you're safe
		agent->ChangeState(new SphEnemyCubeIdleState());
	}
	else
	{
		//do the "fight" action
		((SphEnemyCubeAgent*)agent)->HostileAction(seconds);
	}
}

/**
* Execute (SphEnemyCubeFleeState class)
*
* Executes the "Flee State" action (handles state transition conditions and
*	conditions of execution of current state)
*
* args: agent - The agent controlling the object
*		seconds - The seconds since the last update
*/
void SphEnemyCubeAvoidState::Execute(SphBasicAgent* agent, float seconds) {
	if(!((SphEnemyCubeAgent*)agent)->CheckRunAway())
	{
		//change to patrol state - if you don't need to run away anymore
		((SphEnemyCubeAgent*)agent)->ChangeState(new SphEnemyCubeIdleState());
	}
	else
	{
		//do the "flee" action
		((SphEnemyCubeAgent*)agent)->AvoidAction(seconds);
	}
}

/**
* Execute (SphEnemyCubeIdleState class)
*
* Executes the "Idle State" action (handles state transition conditions and
*	conditions of execution of current state)
*
* args: agent - The agent controlling the object
*		seconds - The seconds since the last update
*/
void SphEnemyCubeIdleState::Execute(SphBasicAgent* agent, float seconds) {
	if(!((SphEnemyCubeAgent*)agent)->CheckIsSafe())
	{
		//change to "fight" state - if you're no longer "safe"
		agent->ChangeState(new SphEnemyCubeHostileState());
	}
	else
	{
		((SphEnemyCubeAgent*)agent)->IdleAction(seconds);
	}
}

/**
* Execute (SphEnemyCubeMoveState class)
*
* Executes the "Move State" action (handles state transition conditions and
*	conditions of execution of current state)
*
* args: agent - The agent controlling the object
*		seconds - The seconds since the last update
*/
void SphEnemyCubeMoveState::Execute(SphBasicAgent* agent, float seconds) {
	if(((SphEnemyCubeAgent*)agent)->CheckDestinationReached())
	{
		//change to idle state
		agent->ChangeState(new SphEnemyCubeIdleState());
	}
	else
	{
		//do the "move" action
		((SphEnemyCubeAgent*)agent)->MoveAction(seconds);
	}
}
