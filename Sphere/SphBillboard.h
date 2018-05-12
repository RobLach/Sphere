#pragma once
#include "SphObject.h"
#include "SphInclude.h"

//SphBillboard is a basic billboard object in the game world.
class SphBillboard : public SphObject{

protected:
	uint m_texture;

	void RenderTexturedSquare();
public:
	SphBillboard();
	virtual bool Load(cchar* directory, cchar* filename);
	virtual void Render() = 0;
};