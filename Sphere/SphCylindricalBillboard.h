#pragma once
#include "SphBillboard.h"
#include "SphInclude.h"

//SphCylindricalBillboard is a billboard object that uses cylindrical billboarding.
class SphCylindricalBillboard : public SphBillboard{

private:
	
public:
	SphCylindricalBillboard();
	void Render();
};