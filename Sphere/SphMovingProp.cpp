#include "SphMovingProp.h"
#include "SphInclude.h"
#include "SphMesh.h"
#include "SphMath.h"
#include "SphMovingPropAgent.h"
#include "SphMovingPropStates.h"
#include <cstring>

SphMovingProp g_movingProps;

/**
* Default constructor for class SphMovingProp
*/
SphMovingProp::SphMovingProp(){
}

/**
* Special constructor for class SphMovingProp
*
* args: mesh - Pointer to a loaded mesh that represents this object
*		magnetic - true if the platform is magnetic, otherwise false
*		idleTime - How long (in seconds) the platform should idle at each endpoint
*		moveSpeed - How fast the platform should move (when it is in motion)
*		numEndPoints - The number of destination endpoints passed in to the "endPoints" cfloat*
*		endPoints - The endpoint data - contains "numEndPoints * N_XYZ" float values,
*						with each triple representing a single endpoint
*/
SphMovingProp::SphMovingProp(SphMesh* mesh, bool magnetic, float idleTime, float moveSpeed, int numEndPoints, cfloat* endPoints){

	this->m_mesh = mesh;
	this->m_magnet = magnetic;

	this->m_floating = true; //necessary for physics...
	this->m_moveSpeed = moveSpeed;
	this->m_secondsSinceLastMove = 0.0f;
	this->m_secondsToStayIdle = idleTime;
	this->m_numEndPoints = numEndPoints;

	m_currentEndPoint = 0;
	this->m_endPoints = new float[m_numEndPoints][N_XYZ];
	
	//"endPoints" is a 1D array of floats, containing "numEndPoints * N_XYZ" float values
	for(int x = 0; x < numEndPoints; x++)
	{
		Set(this->m_endPoints[x], (endPoints+(x*N_XYZ)));
	}

	//set start position to first endpoint, and set destination to next endpoint in the list
	this->SetPosition(this->m_endPoints[0]);
	//always traverse forward (and loop back to beginning after end of list) in base implementation
	this->m_nextEndpointIndexOffset = 1;
	this->m_currentEndPoint = (this->m_currentEndPoint+this->m_nextEndpointIndexOffset) % this->m_numEndPoints;


	SphMovingPropIdleState* startState = new SphMovingPropIdleState();
	m_agent = new SphMovingPropAgent(this, startState);
}

/**
* Update
*
* Updates the object after a given duration
*
* args: seconds - The seconds since the last update
*/
void SphMovingProp::Update(float seconds){
	//update the agent controlling the AI
	this->m_agent->ExecuteState(seconds);
	//Physics is used in the force-based approach to movement, check MoveAction for changes made
	//this->ApplyPhysics(seconds, this->m_frictionFactor, 0.0f, !this->m_floating);
}


/*************************************
*
* IBaseMovingPropAgent Interface
* 
**************************************/

/**
* CheckStayIdle
*
* Checks if the object should remain in its idle state
*
* return - true if the prop should remain idle
*/
bool SphMovingProp::CheckStayIdle(){

	if(this->m_secondsSinceLastMove >= this->m_secondsToStayIdle) {
		//if idle time has expired, return false
		this->m_secondsSinceLastMove = 0;
		return 0;
	}
	
	return 1;
}

/**
* CheckDestinationReached
*
* Checks if the object has reached its destination
*
* return - true if the prop has reached its destination
*/
bool SphMovingProp::CheckDestinationReached(){
	//check if position is equal to destination ("destination" is the current endpoint)
	if(!memcmp(this->GetPosition(), m_endPoints[m_currentEndPoint], N_XYZ*sizeof(float))){
		//set current endpoint to next in list of endpoints (loop around list)
		// ->this will be according to m_nextEndpointIndexOffset
		m_currentEndPoint = (m_currentEndPoint+this->m_nextEndpointIndexOffset) % m_numEndPoints;
		return 1;
	}
	
	return 0;
}

/**
* IdleAction
*
* Performs the object's "idle" action
*
* args: seconds - Seconds since the last update
*/
void SphMovingProp::IdleAction(float seconds){
	//increment time we've been idle
	this->m_secondsSinceLastMove += seconds;
}

/**
* MoveAction
*
* Performs the object's "move" action
*
* args: seconds - Seconds since the last update
*/
void SphMovingProp::MoveAction(float seconds){
	//get the vector in the direction toward the destination
	float forward[N_XYZ], forward2[N_XYZ];
	Subtract(forward, m_endPoints[m_currentEndPoint], this->GetPosition());
	Set(forward2, forward);

	Normalize(forward);
	Multiply(forward, this->m_moveSpeed * seconds);
	//make sure we're not translating past the destination point
	if(Magnitude(forward) < Magnitude(forward2)){
		this->Translate(forward);
	}
	else{
		this->SetPosition(m_endPoints[m_currentEndPoint]);
	}
	
	//below is a more "physics-based" approach, which doesn't work as well...
	/*if(Magnitude(forward) < 0.25f){
		//if we're close to the destination point, just translate to that position and remove velocity
		this->SetPosition(m_endPoints[m_currentEndPoint]);
		this->SetVelocity(g_zeroes);
	}
	else{
		//apply a force in the direction toward the destination
		Normalize(forward);
		Multiply(forward, this->m_moveSpeed);
		this->AddForce(forward);
	}*/
}

/**
* Trigger
*
* Triggers a moving object to start moving (if it's required)
*/
void SphMovingProp::Trigger(){
}

/**
* GetObjType
*
* Returns the type of this object
*/
SphObject::SphObjectType SphMovingProp::GetObjType(){
	return SphObject::MOVING_PROP_OBJ;
}