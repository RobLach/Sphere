#pragma once
#include "SphInclude.h"
#include "SphpAPI.h"
#include "GL/glut.h"

using namespace PAPI;

class SphExplosionParticles
{

protected:
	uint texture;

public:
	static ParticleContext_t P;
	SphExplosionParticles(cchar* directory, cchar* filename, cfloat* position);
	void ComputeParticles(cfloat* position);
	void SetParameters(cfloat* position);
	void Render();

};