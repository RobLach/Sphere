#pragma once
#include "SphInclude.h"
#include "SphpAPI.h"
#include "GL/glut.h"

using namespace PAPI;

class SphSprayParticles
{

protected:
	uint texture;

public:
	static ParticleContext_t P;
	SphSprayParticles(cchar* directory, cchar* filename, cfloat* position);
	void ComputeParticles(cfloat* position, cfloat* direction);
	void Render();

};
