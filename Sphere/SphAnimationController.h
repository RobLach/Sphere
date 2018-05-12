#pragma once
#include <vector>
#include <stdarg.h>
#include "SphInclude.h"
using namespace std;

class SphModel;

//SphAnimationController allows for a smooth animation control.
//Internally it's implemented as a deterministic finite automata.
//By default, state 0 is the starting state.
class SphAnimationController {

protected:
	SphModel* m_model;
	vector<int> m_transitions;
	vector<int> m_actions;
	int m_alphabetSize;
	int m_activeState;
	int m_activeTransition;

public:
	SphAnimationController(int alphabetSize);
	void Update(float seconds);
	void SetTransitions(int state, int animation, ...);
	void SetActiveTransition(int activeTransition);
	void SetModel(SphModel* model);
   int GetNewState();
	int GetActiveState();
};
