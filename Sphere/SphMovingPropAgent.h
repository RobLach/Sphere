#pragma once
#include "SphInclude.h"
#include "SphBasicAgent.h"
#include "SphBasicAgentInterfaces.h"

class SphMovingProp;

class SphMovingPropAgent : public SphBasicAgent , public IBaseMovingPropAgent {

private:
	//this needs to be a SphMovingProp type (can't cast SphObject to "interface")
	SphMovingProp* m_object;
public:
	SphMovingPropAgent(SphMovingProp* propObject, SphBasicState* startState);
	//boolean self-checks
	virtual bool CheckStayIdle();
	virtual bool CheckDestinationReached();
	//States for the agent's Finite State Machine
	virtual void IdleAction(float seconds);
	virtual void MoveAction(float seconds);
};