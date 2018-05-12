#pragma once
#include "SphInclude.h"

//SphCamera is an abstract class representing a camera.
class SphCamera{

protected:

public:
	SphCamera(){};
	virtual void Update(float seconds) = 0;
	virtual void ApplyCamera() = 0;
	virtual cfloat* GetForward() = 0;
	virtual cfloat* GetPosition() = 0;
	virtual void SetInvertPitch(bool invertPitch) = 0;
};
