#include "SphBasicAgent.h"
#include "SphBasicState.h"
#include "SphObject.h"

/**
* Default Constructor for class SphBasicAgent
*/
SphBasicAgent::SphBasicAgent() {
}

/**
* Special Constructor for class SphBasicAgent
*
* args: object - The object that this agent is controlling
*		startState - The state that this agent starts in
*/
SphBasicAgent::SphBasicAgent(SphObject *object, SphBasicState *startState)
{
	this->m_object = object;
	this->m_currentState = startState;
}

/**
* ExecuteState
*
* Executes the action of the current state the agent is in
*
* args: seconds - seconds since last update
*/
void SphBasicAgent::ExecuteState(float seconds)
{
	m_currentState->Execute(this, seconds);
}

/**
* ChangeState
*
* Transitions the agent from its current state into a new state
*
* args: state - The new state to transition the agent into 
*/
void SphBasicAgent::ChangeState(SphBasicState* state)
{
	m_currentState->Exit(this);
	delete m_currentState; //free the last state
	m_currentState = state;
	m_currentState->Enter(this);
}