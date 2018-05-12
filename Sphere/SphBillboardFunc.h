#pragma once
#include "SphInclude.h"

//SphBillboardFunc is a class that performs many different billboard orientations
// Methods defined using code from http://www.lighthouse3d.com/opengl/billboarding/
class SphBillboardFunc {

public:
	SphBillboardFunc();

	static void BeginCylindrical(cfloat* object_position);
	static void BeginSpherical(cfloat* object_position);
	static void BeginCheatingCylindrical(); //added for "cheating"-style billboard
	static void BeginCheatingSpherical(); //added for "cheating"-style billboard
	static void End();
	
	static void BillboardGetUpRightVector(float* up, float* right);
};