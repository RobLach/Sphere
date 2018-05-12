#pragma once
#include "SphInclude.h"
#include "SphAttachment.h"

class SphMesh;
class SphAvatar;

//SphSprayGunAttachment is a SphAttachment for the spray gun.
class SphSprayGunAttachment : public SphAttachment {

protected:
	static SphMesh* s_sprayGunMesh;
	static const int m_sprayCount = 5;

public:
	SphSprayGunAttachment();
	void Update(float seconds);
	void AddAttachment(SphAvatar* avatar);
	void RemoveAttachment(SphAvatar* avatar);
};