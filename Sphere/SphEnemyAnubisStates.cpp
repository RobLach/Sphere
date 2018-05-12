#include "SphInclude.h"
#include "SphEnemyAnubisStates.h"
#include "SphEnemyAnubisAgent.h"

/**
* Execute (SphEnemyAnubisHostileState class)
*
* Executes the "Hostile State" action (handles state transition conditions and
*	conditions of execution of current state)
*
* args: agent - The agent controlling the object
*		seconds - The seconds since the last update
*/
void SphEnemyAnubisHostileState::Execute(SphBasicAgent* agent, float seconds) {
	if(!((SphEnemyAnubisAgent*)agent)->CheckDoAttack())
	{
		//change to idle state if you're done doing your attack
		agent->ChangeState(new SphEnemyAnubisIdleState());
	}
	else
	{
		//do the "fight" action
		((SphEnemyAnubisAgent*)agent)->HostileAction(seconds);
	}
}

/**
* Execute (SphEnemyAnubisIdleState class)
*
* Executes the "Idle State" action (handles state transition conditions and
*	conditions of execution of current state)
*
* args: agent - The agent controlling the object
*		seconds - The seconds since the last update
*/
void SphEnemyAnubisIdleState::Execute(SphBasicAgent* agent, float seconds) {
	if(!((SphEnemyAnubisAgent*)agent)->CheckIsSafe())
	{
		if(((SphEnemyAnubisAgent*)agent)->CheckDoPreAttack())
		{
			//change to pre-attack state
			agent->ChangeState(new SphEnemyAnubisPreAttackState());
		}
		else
		{
			//wait between attacks, but only increment time in between attacks when player is close enough
			((SphEnemyAnubisAgent*)agent)->IdleAction(seconds);
		}
	}
}

/**
* Execute (SphEnemyAnubisMoveState class)
*
* Executes the "Move State" action (handles state transition conditions and
*	conditions of execution of current state)
*
* args: agent - The agent controlling the object
*		seconds - The seconds since the last update
*/
void SphEnemyAnubisMoveState::Execute(SphBasicAgent* agent, float seconds) {
	if(((SphEnemyAnubisAgent*)agent)->CheckDestinationReached())
	{
		//change to idle state
		agent->ChangeState(new SphEnemyAnubisIdleState());
	}
	else
	{
		//do the "move" action
		((SphEnemyAnubisAgent*)agent)->MoveAction(seconds);
	}
}

/**
* Execute (SphEnemyAnubisPreAttackState class)
*
* Executes the "Pre-Attack State" action (handles state transition conditions and
*	conditions of execution of current state)
*
* args: agent - The agent controlling the object
*		seconds - The seconds since the last update
*/
void SphEnemyAnubisPreAttackState::Execute(SphBasicAgent* agent, float seconds) {
	if(((SphEnemyAnubisAgent*)agent)->CheckDoAttack())
	{
		//change to attack state if we're done being in pre-attack state
		agent->ChangeState(new SphEnemyAnubisHostileState());
	}
	else
	{
		//do the "move" action
		((SphEnemyAnubisAgent*)agent)->PreAttackAction(seconds);
	}
}