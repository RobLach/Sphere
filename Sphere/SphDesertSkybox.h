#pragma once
#include "SphInclude.h"
#include "SphSkybox.h"

class SphSkybox;

//SphGround is the class for the ground.
class SphDesertSkybox : public SphSkybox {

public:

	SphSkybox* m_skybox;

	SphDesertSkybox();
	virtual void update(cfloat *pos, float range);
	virtual void render();
};