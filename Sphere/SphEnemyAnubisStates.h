#pragma once
#include "SphInclude.h"
#include "SphBasicState.h"


class SphEnemyAnubisHostileState : public SphBasicState {
public:
	void Execute(SphBasicAgent* agent, float seconds);
};

class SphEnemyAnubisIdleState : public SphBasicState {
public:
	void Execute(SphBasicAgent* agent, float seconds);
};

class SphEnemyAnubisMoveState : public SphBasicState {
public:
	void Execute(SphBasicAgent* agent, float seconds);
};

class SphEnemyAnubisPreAttackState : public SphBasicState {
public:
	void Execute(SphBasicAgent* agent, float seconds);
};