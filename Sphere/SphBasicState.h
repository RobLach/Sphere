#pragma once
#include "SphInclude.h"

class SphBasicAgent;

//Abstract class defining the basic state functions
// (Assuming we use a FSM implementation)
class SphBasicState {

public:
	SphBasicState();

	//Called when the state is entered
	virtual void Enter(SphBasicAgent* agent);
	//Called when the state is executed
	virtual void Execute(SphBasicAgent* agent, float seconds) = 0;
	//Called when the state is exited
	virtual void Exit(SphBasicAgent* agent);
};