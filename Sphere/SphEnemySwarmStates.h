#pragma once
#include "SphInclude.h"
#include "SphBasicState.h"


class SphEnemySwarmHostileState : public SphBasicState {
public:
	void Execute(SphBasicAgent* agent, float seconds);
};

class SphEnemySwarmAvoidState : public SphBasicState {
public:
	void Execute(SphBasicAgent* agent, float seconds);
};

class SphEnemySwarmIdleState : public SphBasicState {
public:
	void Execute(SphBasicAgent* agent, float seconds);
};

class SphEnemySwarmMoveState : public SphBasicState {
public:
	void Execute(SphBasicAgent* agent, float seconds);
};
