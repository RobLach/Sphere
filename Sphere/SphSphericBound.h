#pragma once
#include "SphInclude.h"
#include "SphPointBound.h"

//SphBoundSphere is a SphBound with a sphere bound.
class SphSphericBound : public SphPointBound {

public:
	float m_radius;

	SphSphericBound(SphObject* object, float m_radius = 1.0f);
	void Render();
};
