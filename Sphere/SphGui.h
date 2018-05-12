#pragma once
#include <list>
#include "SphInclude.h"
using namespace std;

class SphGuiObject;

typedef list<SphGuiObject*> SphGuiObjectList;

//SphGui contains a list of SphGuiObjects.
class SphGui{

private:
	SphGuiObjectList m_objects;

public:
	SphGui();
	int Init();
	void Render();
	void Update(float seconds);
	void AddGuiObject(SphGuiObject* object);
	void RemoveGuiObject(SphGuiObject* object);
	void Quit();
};

extern SphGui g_gui;
