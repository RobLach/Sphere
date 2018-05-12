#pragma once
#include "SphInclude.h"
#include "SphGuiObject.h"

//SphGuiImage is a basic texture mapped rectangle.
class SphGuiImage : public SphGuiObject {

protected:

public:
	uint m_texture;

	SphGuiImage();
	void Render();
	bool Load(cchar* directory, cchar* filename);
};
