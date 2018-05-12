#pragma once
#include "SphInclude.h"

//This is a basic FSM-based implementation of game AI

//could put multiple interfaces here?

//This is the basic AI agent interface,
// these need to be implemented when inherited
// (This should be inherited by the object class that is being controlled by AI)
class IBaseEnemyAgent {
public:
	virtual bool CheckIsSafe() = 0;
	virtual bool CheckRunAway() = 0;
	virtual bool CheckDestinationReached() = 0;

	//States for the agent's Finite State Machine
	// feel free to add/remove/modify any of these
	virtual void HostileAction(float seconds) = 0;
	virtual void AvoidAction(float seconds) = 0;
	virtual void IdleAction(float seconds) = 0;
	virtual void MoveAction(float seconds) = 0;
	//I'm not sure what we'll need our AI to do
};

/*
*	ADDED INTERFACE FOR EXTRA BOSS FUNCTIONALITY
*/ 
class IBaseEnemyBossAgent {
public:
	virtual bool CheckDoPreAttack() = 0;
	virtual bool CheckDoAttack() = 0;

	virtual void PreAttackAction(float seconds) = 0;
};

/**
* Basic Agent Interface for Moving Props
*/
class IBaseMovingPropAgent {
public:
	virtual bool CheckStayIdle() = 0;
	virtual bool CheckDestinationReached() = 0;

	//States for the agent's Finite State Machine
	virtual void IdleAction(float seconds) = 0;
	virtual void MoveAction(float seconds) = 0;
};



//A note about implementing AI:
//This implementation requires that...
// 1) the NPC class implements this interface,
// 2) A state class is created for every unique finite state
//		(each will implement the abstract SphBasicState class)
// 3) An agent class is created for the AI type, so that it can be used by the class in #2
//		(this implements the BasicAgent class).  It may call the functions defined in the NPC class