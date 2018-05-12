#pragma once
#include "SphInclude.h"
#include "SphMesh.h"

//SphProp is an SphObject associated with a mesh, which is created static to instace classes.
class SphProp : public SphObject {

protected:
	SphMesh* m_mesh;
	float m_bounceFactor;
	float m_frictionFactor;
	bool m_magnet;

public:
	SphProp(SphMesh* mesh = NULL);
	void SetMesh(SphMesh* mesh);
	void Render();
	SphObjectType GetObjType();

	float GetBounceFactor();
	float GetFrictionFactor();
	bool IsMagnet();
};