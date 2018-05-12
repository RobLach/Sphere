#pragma once
#include "SphInclude.h"
#include "SphItem.h"

class SphMesh;

//SphMedpack is a SphItem which will give avatar health points upon collision.
class SphAbilityItem : public SphItem {

	enum AbilityItemType { Legs, Laser, Spray, Rubber, Speed, Metal };

private:
	float m_bobAmplitude;
	float m_bobFrequency;

protected:
	SphMesh m_legs, m_laser, m_spray, m_rubber, m_speed, m_metal;
	AbilityItemType m_abilityItemType;

public:
	SphAbilityItem();
	void CreateAbilityItem(SphAbilityItem* item, cchar* name);
	void ItemPickup(SphAvatar* avatar);
	SphMesh* GetMesh();
	void Init();
	SphAbilityItem* CreateNewProp(cchar* name);
	void Update(float seconds);

	void SetBobAmplitude(cfloat bobAmplitude);
	void SetBobFrequency(cfloat bobFrequency);
};

extern SphAbilityItem g_abilityItems;
