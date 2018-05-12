#include "SphInclude.h"
#include "SphEnemySwarmStates.h"
#include "SphEnemySwarmAgent.h"

/**
* Execute (SphEnemySwarmHostileState class)
*
* Executes the "Hostile State" action (handles state transition conditions and
*	conditions of execution of current state)
*
* args: agent - The agent controlling the object
*		seconds - The seconds since the last update
*/
void SphEnemySwarmHostileState::Execute(SphBasicAgent* agent, float seconds) {
	if(((SphEnemySwarmAgent*)agent)->CheckRunAway())
	{
		//change to flee state if you need to run away
		agent->ChangeState(new SphEnemySwarmAvoidState());
	}
	else if(((SphEnemySwarmAgent*)agent)->CheckIsSafe())
	{
		//change to idle state if you're safe
		agent->ChangeState(new SphEnemySwarmIdleState());
	}
	else
	{
		//do the "fight" action
		((SphEnemySwarmAgent*)agent)->HostileAction(seconds);
	}
}

/**
* Execute (SphEnemySwarmFleeState class)
*
* Executes the "Flee State" action (handles state transition conditions and
*	conditions of execution of current state)
*
* args: agent - The agent controlling the object
*		seconds - The seconds since the last update
*/
void SphEnemySwarmAvoidState::Execute(SphBasicAgent* agent, float seconds) {
	if(!((SphEnemySwarmAgent*)agent)->CheckRunAway())
	{
		//change to patrol state - if you don't need to run away anymore
		((SphEnemySwarmAgent*)agent)->ChangeState(new SphEnemySwarmIdleState());
	}
	else
	{
		//do the "flee" action
		((SphEnemySwarmAgent*)agent)->AvoidAction(seconds);
	}
}

/**
* Execute (SphEnemySwarmIdleState class)
*
* Executes the "Idle State" action (handles state transition conditions and
*	conditions of execution of current state)
*
* args: agent - The agent controlling the object
*		seconds - The seconds since the last update
*/
void SphEnemySwarmIdleState::Execute(SphBasicAgent* agent, float seconds) {
	if(!((SphEnemySwarmAgent*)agent)->CheckIsSafe())
	{
		//change to "fight" state - if you're no longer "safe"
		agent->ChangeState(new SphEnemySwarmHostileState());
	}
	else
	{
		((SphEnemySwarmAgent*)agent)->IdleAction(seconds);
	}
}

/**
* Execute (SphEnemySwarmMoveState class)
*
* Executes the "Move State" action (handles state transition conditions and
*	conditions of execution of current state)
*
* args: agent - The agent controlling the object
*		seconds - The seconds since the last update
*/
void SphEnemySwarmMoveState::Execute(SphBasicAgent* agent, float seconds) {
	if(((SphEnemySwarmAgent*)agent)->CheckDestinationReached())
	{
		//change to idle state
		agent->ChangeState(new SphEnemySwarmIdleState());
	}
	else
	{
		//do the "move" action
		((SphEnemySwarmAgent*)agent)->MoveAction(seconds);
	}
}
