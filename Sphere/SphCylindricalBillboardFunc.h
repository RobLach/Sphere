#pragma once
#include "SphInclude.h"
#include "SphBillboardFunc.h"

//SphCylindricalBillboardFunc is a class that performs cylindrical billboard orientation
class SphCylindricalBillboardFunc : public SphBillboardFunc {

private:

public:
	SphCylindricalBillboardFunc();
	void Begin(cfloat* object_position);
	void CheatingBegin(); //added for "cheating"-style billboard
	void End();
};
	
extern SphCylindricalBillboardFunc g_cylinderBillboardFunc;
