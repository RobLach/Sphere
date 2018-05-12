#pragma once
#include "SphInclude.h"
#include "SphProp.h"

class SphAvatar;

//SphItem is SphProp which can be picked up by avatar.
class SphItem : public SphProp {

protected:

public:
	static SphItem* CreateNewProp(cchar* name);

	SphItem();
	virtual void ItemPickup(SphAvatar* avatar) = 0;
	SphObject::SphObjectType GetObjType();
};

