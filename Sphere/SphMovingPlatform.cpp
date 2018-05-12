#include "SphMovingPlatform.h"
#include "SphMovingPropAgent.h"
#include "SphMovingPropStates.h"
#include "SphSphericBound.h"
#include "SphTightBound.h"
#include "SphMesh.h"
#include "SphMath.h"
#include <cstring>

SphMovingPlatform g_movingPlatforms;

bool waitForTrigger;	//the platform needs to wait for a trigger before moving
int times_triggered;	//number of times this platform has been triggered

/**
* Default constructor for class SphMovingPlatform
*/
SphMovingPlatform::SphMovingPlatform(){
	this->m_secondsSinceLastMove = 0.0f;
	this->m_secondsToStayIdle = 1.0f;
	this->m_floating = true;
	this->m_moveSpeed = 20.0f;
	this->m_nextEndpointIndexOffset = 1; // should always be (+/- 1)
	waitForTrigger = false;
	times_triggered = 0;
}

/**
* Special constructor for class SphMovingPlatform
*
* args: mesh - The previosly-loaded mesh to use for this object
*		triggeredStart - true if the object must be triggered in some way in order for it to start moving
*					(Trigger method called to trigger to object)
*		maxTriggers - The maximum number of times that the object may be triggered
*					(note: "triggers" only accumulate when the object is waiting to be triggered - eg, stopped entirely)
*		magnetic - true if the object is magnetic, otherwise false
*		idleTime - Duration (in seconds) that the platform should remain idle when it reaches one of its endpoints
*		moveSpeed - How fast (in units/second) the platform should move between destinations
*		numEndPoints - The number of destination endpoints being passed to this object
*		endPoints - The destination endpoints (as a 1D array, each endpoint represented as a triple)
*
* returns: A SphMovingPlatform object which contains the properties specified
*/
SphMovingPlatform::SphMovingPlatform(SphMesh* mesh, bool triggeredStart, int maxTriggers, bool magnetic, float idleTime, float moveSpeed, int numEndPoints, cfloat* endPoints)
							:SphMovingProp(mesh, magnetic, idleTime, moveSpeed, numEndPoints, endPoints)
{
	this->m_triggeredStart = triggeredStart;
	this->m_maxTriggers = maxTriggers;
	this->m_floating = true;
	this->m_nextEndpointIndexOffset = 1;		// should always be (+/- 1)
	waitForTrigger = this->m_triggeredStart;	// don't move right away if this needs to be triggered
	times_triggered = 0;						// obviously hasn't been triggered initially
}

/**
* Init
*
* Load meshes to be used for SphMovingPlatform.
*/
void SphMovingPlatform::Init(){
	//currently just using "wall3" mesh and tight bound (see below)
	m_regular.Load("Meshes/Walls/", "Puzzle4a.drkMesh");
}

/**
* CreateMovingPlatform
*
* Creates a SphMovingPlatform object by re-using a previously-loaded mesh
*
* args: mesh - The previosly-loaded mesh to use for this object
*		boundFilepath - The filepath of the "tight bound" to use for the object
*		triggeredStart - true if the object must be triggered in some way in order for it to start moving
*					(Trigger method called to trigger to object)
*		maxTriggers - The maximum number of times that the object may be triggered
*					(note: "triggers" only accumulate when the object is waiting to be triggered - eg, stopped entirely)
*		magnetic - true if the object is magnetic, otherwise false
*		idleTime - Duration (in seconds) that the platform should remain idle when it reaches one of its endpoints
*		moveSpeed - How fast (in units/second) the platform should move between destinations
*		numEndPoints - The number of destination endpoints being passed to this object
*		endPoints - The destination endpoints (as a 1D array, each endpoint represented as a triple)
*
* returns: A SphMovingPlatform object which contains the properties specified
*/
SphMovingPlatform* SphMovingPlatform::CreateMovingPlatform(SphMesh* mesh, cchar* boundFilepath, bool triggeredStart, int maxTriggers, bool magnetic, float idleTime, float moveSpeed, int numEndPoints, cfloat* endPoints){
	
	SphMovingPlatform* platform = new SphMovingPlatform(mesh, triggeredStart, maxTriggers, magnetic, idleTime, moveSpeed, numEndPoints, endPoints);
	platform->m_bound = new SphTightBound(platform);
	((SphTightBound*)platform->m_bound)->Load(boundFilepath);
	platform->SetPosition(platform->GetPosition());
	//platform->SetBound(new SphSphericBound(platform, 1.5f)); //test spheric bound

	return platform;
}

/**
* CreateNewProp
*
* Creates a SphMovingPlatform object using a specified "name" type and other properties
*
* args:	name - The name (type) of platform to create.  (Current Types: "regular", "magnet")
*		idleTime - Duration (in seconds) that the platform should remain idle when it reaches one of its endpoints
*		moveSpeed - How fast (in units/second) the platform should move between destinations
*		numEndPoints - The number of destination endpoints being passed to this object
*		endPoints - The destination endpoints (as a 1D array, each endpoint represented as a triple)
*
* returns: A SphMovingPlatform object which contains the properties specified, if "name" matches with one of the specified platform types
*			Else NULL
*/
SphMovingPlatform* SphMovingPlatform::CreateNewProp(cchar* name, float idleTime, float moveSpeed, int numEndPoints, cfloat* endPoints){
	
	if(!strcmp(name, "regular")) //traverses forward, then backward through endpoint list forever
	{ 
		SphMovingPlatform* platform = CreateMovingPlatform(&this->m_regular, "Meshes/Walls/Puzzle4a.bound.drkMesh", false, 0, false, idleTime, moveSpeed, numEndPoints, endPoints);
		platform->m_platformType = Regular;
		return platform;
	}
	if(!strcmp(name, "magnet"))	//same as regular, but magnetic?
	{ 
		SphMovingPlatform* platform = CreateMovingPlatform(&this->m_regular, "Meshes/Walls/Puzzle4a.bound.drkMesh", false, 0, true, idleTime, moveSpeed, numEndPoints, endPoints);
		platform->m_platformType = Magnet;
		return platform;
	}
	if(!strcmp(name, "triggered_oneway"))	//only goes to last endpoint, and never returns
	{
		SphMovingPlatform* platform = CreateMovingPlatform(&this->m_regular, "Meshes/Walls/Puzzle4a.bound.drkMesh", true, 1, false, idleTime, moveSpeed, numEndPoints, endPoints);
		platform->m_platformType = Regular;
		return platform;
	}
	if(!strcmp(name, "triggered_twoway"))	//hits last endpoint, but can be triggered again to traverse backward
	{
		SphMovingPlatform* platform = CreateMovingPlatform(&this->m_regular, "Meshes/Walls/Puzzle4a.bound.drkMesh", true, 2, false, idleTime, moveSpeed, numEndPoints, endPoints);
		platform->m_platformType = Regular;
		return platform;
	}


	return NULL;
}

/**
* CheckStayIdle
*
* Checks if the object should remain in its idle state
*
* return - true if the prop should remain idle
*/
bool SphMovingPlatform::CheckStayIdle(){

	if(this->m_secondsSinceLastMove >= this->m_secondsToStayIdle) {
		//if idle time has expired, return false
		if(!waitForTrigger){
			this->m_secondsSinceLastMove = 0;
			return 0;
		}
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
bool SphMovingPlatform::CheckDestinationReached(){
	//check if position is equal to destination ("destination" is the current endpoint)
	if(!memcmp(this->GetPosition(), m_endPoints[m_currentEndPoint], N_XYZ*sizeof(float))){
		//set current endpoint to next in list of endpoints
		//	(traverse backward through list if we've reached the first or last element)
		if((m_currentEndPoint == 0) || (m_currentEndPoint == this->m_numEndPoints-1)){
			//If this is a triggered platform, make the platform wait until it is triggered
			if(this->m_triggeredStart){
				waitForTrigger = true;
			}

			this->m_nextEndpointIndexOffset = (this->m_nextEndpointIndexOffset == 1) ? -1 : 1;
		}
		m_currentEndPoint = (m_currentEndPoint + this->m_nextEndpointIndexOffset);
		return 1;
	}
	
	return 0;
}

/**
* Trigger
*
* Triggers the platform to start moving
*	(number of triggers used only increments if the platform is currently waiting for a trigger)
*/
void SphMovingPlatform::Trigger(){
	//start moving if waiting for trigger -AND-
	//	(not yet triggered max amount of times)
	if(waitForTrigger && (times_triggered < this->m_maxTriggers)){
		times_triggered++;
		waitForTrigger = false;
	}
}