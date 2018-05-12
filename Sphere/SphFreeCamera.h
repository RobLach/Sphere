#pragma once
#include "SphInclude.h"
#include "SphCamera.h"

//SphFreeCamera provides a camera, which can be controlled directly by the user.
class SphFreeCamera : public SphCamera {

public:
	float m_position[N_XYZ];
	float m_rotationY;//in radians
	float m_pitch;//in radians

	SphFreeCamera();
	void ApplyCamera();
	void Update(float seconds);
	cfloat* GetForward();
};
