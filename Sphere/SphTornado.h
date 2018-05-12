#pragma once
#include "SphInclude.h"
#include "SphpAPI.h"

using namespace PAPI;

class SphTornado
{

protected:
	uint texture;
	float *ppos;


public:
	static ParticleContext_t P;
	SphTornado();
	void ComputeParticles();
	void DrawGroupAsPoints();
};
