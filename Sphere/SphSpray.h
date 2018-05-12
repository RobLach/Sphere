#pragma once
#include "SphInclude.h"
#include "SphProjectile.h"
#include "SphModel.h"
#include "SphMesh.h"
#include "SphSprayParticles.h"

class SphMesh;

//SphSpray is a SphProjectile for SphSprayGunAttachment.
class SphSpray : public SphProjectile {

private:
	static SphMesh* s_sprayMesh;
	SphSprayParticles* m_particles;
	
protected:

	void ProcessCollisions();
	void ProcessCollision(SphCollisionInfo* info);

public:
	SphSpray();
	void Update(float seconds);
	void Render();
};