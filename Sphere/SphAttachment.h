#pragma once
#include "SphInclude.h"

class SphAvatar;

//SphAttachment is an open form attachment for the avatar.
class SphAttachment{

protected:
	bool m_attached;

public:
	SphAttachment();
	virtual void Update(float seconds) = 0;
	virtual void AddAttachment(SphAvatar* avatar);
	virtual void RemoveAttachment(SphAvatar* avatar);
	bool IsAttached();
};