#pragma once
#include "SphInclude.h"
#include "SphProjectile.h"
#include "SphModel.h"
#include "SphMesh.h"

class SphMesh;

//SphLaser is a SphProjectile for SphLaserGunAttachment.
class SphLaser : public SphProjectile {

private:
	static SphMesh* s_laserMesh;
	

protected:
	void ProcessCollisions();
	void ProcessCollision(SphCollisionInfo* info);

public:
	SphLaser();
	void Update(float seconds);
};