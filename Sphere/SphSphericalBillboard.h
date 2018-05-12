#pragma once
#include "SphBillboard.h"
#include "SphInclude.h"

//SphSphericalBillboard is a billboard object that uses spherical billboarding.
class SphSphericalBillboard : public SphBillboard {

private:
	
public:
	SphSphericalBillboard();
	void Render();
};