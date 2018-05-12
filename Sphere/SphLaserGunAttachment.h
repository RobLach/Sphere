#pragma once
#include "SphInclude.h"
#include "SphAttachment.h"

class SphMesh;
class SphAvatar;

//SphLaserGunAttachment is a SphAttachment for the laser gun.
class SphLaserGunAttachment : public SphAttachment {

protected:
	static SphMesh* s_laserGunMesh;

public:
	SphLaserGunAttachment();
	void Update(float seconds);
	void AddAttachment(SphAvatar* avatar);
	void RemoveAttachment(SphAvatar* avatar);
};