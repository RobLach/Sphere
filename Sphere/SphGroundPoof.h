#pragma once
#include "SphInclude.h"
#include "SphpAPI.h"

using namespace PAPI;

class SphGroundPoof
{

protected:
	uint texture;
	float *ppos;


public:
	static ParticleContext_t P;
	SphGroundPoof();
	void ComputeParticles();
	void DrawGroupAsPoints();
	void ResetPuff(cfloat* pos);
};
