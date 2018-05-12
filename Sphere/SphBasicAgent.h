#pragma once
#include "SphInclude.h"

class SphObject;
class SphBasicState;

//Abstract class defining the Base Agent.
//  All agents are built from this template.
//  All new AI should inherit from this class.
class SphBasicAgent {

protected:
	int m_agentId;					//Unique ID for this agent
	SphBasicState* m_currentState;	//This agent's current state
	SphObject* m_object;			//The object currently associated with this agent

private:
	void SetID();
public:
	SphBasicAgent();
	SphBasicAgent(SphObject* object, SphBasicState* startState);
	//state transition & execution functions
	virtual void ChangeState(SphBasicState* newState);
	void ExecuteState(float seconds);
};