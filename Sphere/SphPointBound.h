#pragma once
#include "SphInclude.h"
#include "SphBound.h"

//SphPointBound is a SphBound with a point bound.
class SphPointBound : public SphBound {

protected:
	float m_oldPosition[N_XYZ];

public:
	SphPointBound(SphObject* object);
	void Render();
	void UpdateBound();
	cfloat* GetOldPosition();
};
