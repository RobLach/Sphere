#include <stdarg.h>
#include "SphMath.h"
#include "SphAnimationController.h"
#include "SphOpenAL.h"
#include "SphModel.h"
#include "SphAction.h"

//Create SphAnimationController.
SphAnimationController::SphAnimationController(int alphabetSize){
	this->m_alphabetSize = alphabetSize;
	this->m_model = NULL;
	this->m_activeState = 0;
}

//Update SphAnimationController.
//seconds - since the last call.
void SphAnimationController::Update(float seconds){
	SphAction* curAction = m_model->GetAction(m_actions[m_activeState]);
	bool active = curAction->IsActive();
	float overflowTime = curAction->GetOverflowTime();
	if(active && overflowTime == 0.0f) return;

	int nextState = m_transitions[m_activeState * m_alphabetSize + m_activeTransition];
	if(active && nextState == m_activeState) return;

	m_model->StopAction(m_actions[m_activeState], true);
	m_activeState = nextState;
	bool loop = (m_transitions[m_activeState * m_alphabetSize] == m_activeState);
	m_model->PlayAction(m_actions[m_activeState], 0.0f, loop);
}

//state - set the transitions for this state.
//animation - index for the animation.
//... - list of transitions, where first transition corresponds to the first alphabet symbol and so on, AlphabetSize in total.
void SphAnimationController::SetTransitions(int state, int animation, ...){
	va_list vaList;
	va_start(vaList, animation);
	int indexOffset = state * m_alphabetSize;
	for(int n = 0; n < m_alphabetSize; n++){
		m_transitions[indexOffset + n] = va_arg(vaList, int);
	}
	m_actions[state] = animation;
	va_end(vaList);
}

//activeTransition - will be followed as soon as the current animation is over.
void SphAnimationController::SetActiveTransition(int activeTransition){
	if(m_activeTransition != activeTransition)	{
		m_activeTransition = activeTransition;
	}
}

//model - to query for state of animations.
void SphAnimationController::SetModel(SphModel* model){
	m_model = model;
}

//return - index value for a new animation state.
int SphAnimationController::GetNewState(){
	for(int n = 0; n < m_alphabetSize; n++){
		m_transitions.push_back(0);
	}
	m_actions.push_back(0);
	return m_actions.size() - 1;
}

//return - current active state.
int SphAnimationController::GetActiveState(){
	return m_activeState;
}