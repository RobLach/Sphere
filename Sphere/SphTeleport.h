#pragma once
#include "SphInclude.h"
#include "SphSphericalBillboard.h"

class SphTeleport : public SphSphericalBillboard{

protected:
	float m_dest[N_XYZ];

public:
	SphTeleport(float x, float y, float z);
	void Update(float seconds);
};