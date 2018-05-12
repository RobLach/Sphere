#pragma once
#include <stdio.h>
#include "SphInclude.h"

//SphMaterial stores material and texture parameters.
class SphMaterial{

public:
	float m_diffuse[N_RGBA];
	float m_specular[N_RGBA];
	float m_shininess;
	int m_texture;

	SphMaterial();
	void Load(FILE* file);
	void LoadTexture(cchar* filePath);
	void ActivateMaterial();
};
