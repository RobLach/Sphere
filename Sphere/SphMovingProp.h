#pragma once
#include "SphInclude.h"
#include "SphProp.h"
#include "SphBasicAgentInterfaces.h"

class SphBasicAgent;
class SphMesh;

//SphMovingProp class, creates a special type of props which have AI to control movement
class SphMovingProp : public SphProp, public IBaseMovingPropAgent {

public:
	SphBasicAgent* m_agent;

protected:
	bool m_floating;
	//how fast to move
	float m_moveSpeed;
	//how long to stay idle when prop has reached an endpoint
	float m_secondsToStayIdle;
	float m_secondsSinceLastMove;
	//where/how to move
	int m_currentEndPoint;
	int m_nextEndpointIndexOffset;	//the index of the next endpoint relative to the current one
	int m_numEndPoints;
	float (*m_endPoints)[N_XYZ];

public:
	SphMovingProp();
	SphMovingProp(SphMesh* mesh, bool magnetic, float idleTime, float moveSpeed, int numEndPoints, cfloat* endPoints);

	virtual void Update(float seconds);

	//IBaseMovingPropAgent Interface
	virtual bool CheckStayIdle();
	virtual bool CheckDestinationReached();
	virtual void IdleAction(float seconds);
	virtual void MoveAction(float seconds);
	//AI-related functions
	virtual void Trigger();	//for triggerable functions


	virtual SphObject::SphObjectType GetObjType();
};

extern SphMovingProp g_movingProps;