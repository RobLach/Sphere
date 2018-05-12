#pragma once
#include "SphInclude.h"
#include "SphItem.h"

class SphMesh;

//SphCactus is a SphItem which acts as a collectible for the avatar to collect.
class SphCactus : public SphItem {

protected:
	SphMesh m_cactus;

public:
	SphCactus();
	void CreateCactus(SphCactus* cactus, cchar* name);
	void ItemPickup(SphAvatar* avatar);
	SphMesh* GetMesh();
	void Init();
	SphCactus* CreateNewProp(cchar* name);
};

extern SphCactus g_cacti;