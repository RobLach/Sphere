#pragma once
#include "SphInclude.h"
#include "SphMovingProp.h"

class SphMesh;

class SphMovingPlatform : public SphMovingProp {
	enum PlatformType { Regular, Magnet };
protected:
	SphMesh m_regular;
	PlatformType m_platformType;
	//extra properties added for moving platforms
	bool m_triggeredStart;
	int m_maxTriggers;

	SphMovingPlatform* CreateMovingPlatform(SphMesh* mesh, cchar* boundFilepath, bool triggeredStart, int maxTriggers, bool magnetic, float idleTime, float moveSpeed, int numEndPoints, cfloat* endPoints);

public:
	SphMovingPlatform();
	SphMovingPlatform(SphMesh* mesh, bool triggered, int maxTriggers, bool magnetic, float idleTime, float moveSpeed, int numEndPoints, cfloat* endPoints);
	void Init();
	SphMovingPlatform* CreateNewProp(cchar* name, float idleTime, float moveSpeed, int numEndPoints, cfloat* endPoints);

	//Special overridden AI-related functions (IBaseMovingPlatformAgent)
	bool CheckStayIdle();
	bool CheckDestinationReached();
	//Methods important to triggering of AI
	void Trigger();

};

extern SphMovingPlatform g_movingPlatforms;