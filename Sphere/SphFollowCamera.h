#pragma once
#include "SphInclude.h"
#include "SphCamera.h"

class SphObject;

//SphFollowCamera provides a camera, which follows an object, facing the same direction as the object.
class SphFollowCamera : public SphCamera {

public:
	SphObject* m_object;
	
	float m_followDistance;
	float m_cameraHeight;
	float m_lookAtHeight;
	float m_targetHeight;

	//let's give the camera its own vectors
	float m_lookAt[N_XYZ];
	float m_position[N_XYZ];
	float m_forward[N_XYZ]; 

	float m_rotationY;
	float m_rotationPitch;
	bool m_invertPitch;
	
	float m_forwardMovement[N_XYZ]; //added for testing force vectors

	SphFollowCamera();
	void ApplyCamera();
	void Update(float seconds);
	void SetParams(float followDistance, float cameraHeight, float lookAtHeight);
	void SetInvertPitch(bool invertPitch);

	//temporary functions for testing the application of force vectors relative to camera
	cfloat* GetForward();
	cfloat* GetLookAt();
	cfloat* GetPosition();
	cfloat* GetForwardMovement();
	void RenderForwardMovement(); //just for test
};
