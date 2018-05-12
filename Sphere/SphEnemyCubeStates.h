#pragma once
#include "SphInclude.h"
#include "SphBasicState.h"


class SphEnemyCubeHostileState : public SphBasicState {
public:
	void Execute(SphBasicAgent* agent, float seconds);
};

class SphEnemyCubeAvoidState : public SphBasicState {
public:
	void Execute(SphBasicAgent* agent, float seconds);
};

class SphEnemyCubeIdleState : public SphBasicState {
public:
	void Execute(SphBasicAgent* agent, float seconds);
};

class SphEnemyCubeMoveState : public SphBasicState {
public:
	void Execute(SphBasicAgent* agent, float seconds);
};
