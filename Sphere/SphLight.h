#pragma once
#include "SphInclude.h"
#include "SphObject.h"
#include "SphOpenGL.h"

//SphLight is the class for the ambient light.
class SphLight : public SphObject{

protected:
	float ambientLight[N_QUAD];
	float diffuseLight[N_QUAD];
	float specularLight[N_QUAD];
	float lightPosition[N_QUAD];

public:
	SphLight();
	void Render();
	void Update(float seconds);
};