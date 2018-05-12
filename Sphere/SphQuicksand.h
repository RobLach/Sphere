#pragma once
#include "SphInclude.h"
#include "SphMesh.h"
#include "SphProp.h"

//SphQuicksand creates SphProps and loads appropriates meshes for them.
class SphQuicksand : public SphProp{

protected:
	SphMesh m_quicksand1;
	float m_killPlane; //y-value for the "kill plane"
	float m_quicksandPlane; //y-value for the "quicksand plane"

public:
	SphQuicksand();
	void Update(float seconds);
};
