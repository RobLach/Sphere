#pragma once
#include "SphInclude.h"
#include "SphAttachment.h"

class SphMesh;
class SphAvatar;

//SphLegsAttachment is a SphAttachment for the legs.
class SphLegsAttachment : public SphAttachment {

protected:
	static SphMesh *s_upperLegs[N_PAIR], *s_lowerLegs[N_PAIR];

public:
	SphLegsAttachment();
	void Update(float seconds);
	void AddAttachment(SphAvatar* avatar);
	void RemoveAttachment(SphAvatar* avatar);
};