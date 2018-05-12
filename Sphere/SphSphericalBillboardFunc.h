#pragma once
#include "SphInclude.h"
#include "SphBillboardFunc.h"

//SphSphericalBillboardFunc is a class that performs spherical billboard orientation
class SphSphericalBillboardFunc : public SphBillboardFunc {

private:

public:
	SphSphericalBillboardFunc();
	void Begin(cfloat* object_position);
	void CheatingBegin(); //added for "cheating"-style billboard
	void End();
};
	
extern SphSphericalBillboardFunc g_sphereBillboardFunc;