#pragma once
#include "SphInclude.h"
#include "SphpAPI.h"
#include "GL/glut.h"

using namespace PAPI;

class SphFountain
{

protected:
	uint texture;

public:
	static ParticleContext_t P;
	SphFountain(cchar* directory, cchar* filename);
	void ComputeParticles();
	void DrawGroupAsPoints(cfloat* pos);

};