#pragma once
#include "SphInclude.h"
#include "SphBasicState.h"

class SphBasicAgent;

class SphMovingPropIdleState : public SphBasicState {
public:
	void Execute(SphBasicAgent* agent, float seconds);
};

class SphMovingPropMoveState : public SphBasicState {
public:
	void Execute(SphBasicAgent* agent, float seconds);
};
