#pragma once
#include "SphInclude.h"
#include "SphItem.h"

class SphMesh;

//SphKey is a SphItem which will give avatar a key for unlocking doors.
class SphKey : public SphItem {

public:
	enum keyType { Red, Orange, Yellow, Green, Blue, Indigo, Violet };

protected:
	static SphMesh *s_redKeyMesh;
	keyType m_key;

	SphMesh m_redKey, m_orangeKey, m_yellowKey, m_greenKey, m_blueKey, m_indigoKey, m_violetKey;

public:

	SphKey();
	SphKey(keyType type);
	void CreateKey(SphKey* color, cchar* name);
	void ItemPickup(SphAvatar* avatar);
	SphMesh* GetMesh();

	void Init();
	SphKey* CreateNewProp(cchar* name);
};

extern SphKey g_keys;
