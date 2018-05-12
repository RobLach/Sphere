#pragma once
#include "SphInclude.h"
#include "SphItem.h"

class SphMesh;

//SphMedpack is a SphItem which will give avatar health points upon collision.
class SphMedpack : public SphItem {

protected:
	int m_healthValue;
	SphMesh m_small, m_medium, m_full;

public:
	SphMedpack();
	void CreateMedpack(SphMedpack* pack, cchar* name);
	void ItemPickup(SphAvatar* avatar);
	SphMesh* GetMesh();
	void Init();
	SphMedpack* CreateNewProp(cchar* name);
};

extern SphMedpack g_medpacks;
