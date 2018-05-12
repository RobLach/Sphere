#pragma once
#include "SphInclude.h"

//SphGuiObject is an abstract GUI object.
class SphGuiObject{

protected:

public:
	float m_left[N_XY], m_right[N_XY];
	float m_frameColor[N_RGBA];
	float m_frameLineWidth;
	bool  m_drawFrame;

	SphGuiObject();
	virtual void Render();
	virtual void Update(float seconds);
	virtual ~SphGuiObject();
};
