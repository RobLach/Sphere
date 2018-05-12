#pragma once
#include "SphInclude.h"

class SphObject;

//SphBound is an abstract class for a bound.
class SphBound {

public:
	enum BoundType { None, Point, Sphere, Tight };

protected:
	SphObject* m_object;
	BoundType m_boundType;

public:
	SphBound(SphObject* object);
	virtual void Render() = 0;
	virtual void UpdateBound() = 0;
	SphObject* GetObject();
	BoundType GetBoundType();
};
