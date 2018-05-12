#include "SphFollowCamera.h"
#include "SphMath.h"
#include "SphOpenGL.h"
#include "SphObject.h"
#include "SphSdlInput.h"
#include "SphSdl.h"
#include "SphGame.h"
#include "SphAvatar.h"

//temp include
#include "SphRenderer.h"

float l_rotationSpeed = 0.002f;
float l_pitchRotationSpeed = 0.002f;
float l_zoomSpeed = 0.004f;

#define MAX_CAM_PITCH  0.025f
#define MIN_CAM_PITCH -1.5f
#define MAX_FOLLOW_DISTANCE 15.0f
#define MIN_FOLLOW_DISTANCE 3.0f

//Create SphFollowCamera.
SphFollowCamera::SphFollowCamera(){
	this->m_object = NULL;
	this->m_rotationY = PI;
	this->m_rotationPitch = Clamp(0.f, MIN_CAM_PITCH, MAX_CAM_PITCH);
	this->m_invertPitch = false;
	Set(this->m_lookAt, g_zeroes);
	Set(this->m_forward, g_zeroes);
	Set(this->m_position, g_zeroes);

	Set(this->m_forwardMovement, g_zeroes);
}

//Update SphFollowCamera.
//seconds - since the last call.
void SphFollowCamera::Update(float seconds){

	if(!g_sdlInput.IsMouseActive()){
		int xCenter = g_sdl.GetScreenWidth()/2;
		int yCenter = g_sdl.GetScreenHeight()/2;
		
		g_sdlInput.MoveMouseTo(xCenter, yCenter);
		
		if(g_game.m_avatar->IsClosed() || g_game.m_avatar->HasAttachment(SphAvatar::Legs)){
			m_rotationY += l_rotationSpeed * (xCenter - g_sdlInput.GetMouseX());
		}
		if(g_sdlInput.IsKeyDown(SDLK_LCTRL))
		{
			if(m_followDistance <= MAX_FOLLOW_DISTANCE && m_followDistance >= MIN_FOLLOW_DISTANCE)
			{
				m_followDistance -= l_zoomSpeed * (yCenter - g_sdlInput.GetMouseY());
				
				if( m_followDistance > MAX_FOLLOW_DISTANCE)
				{
					m_followDistance = MAX_FOLLOW_DISTANCE;
				}	
				else if(m_followDistance < MIN_FOLLOW_DISTANCE)
				{
					m_followDistance = MIN_FOLLOW_DISTANCE;
				}
			}
		}
		else //if(g_sdlInput.IsKeyDown(SDLK_LSHIFT)) //(for testing...)
		{
			if(m_invertPitch)
				m_rotationPitch -= l_pitchRotationSpeed * (yCenter - g_sdlInput.GetMouseY());
			else
				m_rotationPitch += l_pitchRotationSpeed * (yCenter - g_sdlInput.GetMouseY());

			if( m_rotationPitch > MAX_CAM_PITCH)
			{
				m_rotationPitch = MAX_CAM_PITCH;
			}	
			else if(m_rotationPitch < MIN_CAM_PITCH)
			{
				m_rotationPitch = MIN_CAM_PITCH;
			}
		}
	}
	//update the "forward movement" vector (for physics applications)
}

//Apply camera settings to OpenGL matrix.
void SphFollowCamera::ApplyCamera(){
	
	
	
	//If player wants to tilt camera upward even more,
	//	temporarily change follow distance (fake collide with ground so we can see Anubis)
	float followDistance, lookAtHeight, rotationPitch;
	if(m_rotationPitch > 0.0f)
	{
		//scale the distance
		followDistance = m_followDistance * (1.f - m_rotationPitch);
		float upAngle = Acos(followDistance / m_followDistance);
		lookAtHeight = m_followDistance * Sin(upAngle) + m_lookAtHeight;
		//stop it from actually rotating any further
		rotationPitch = 0.0f;
	}
	else
	{
		followDistance = m_followDistance;
		lookAtHeight = m_lookAtHeight;
		rotationPitch = m_rotationPitch;
	}

	Set(m_position, 0.f, 0.f, followDistance);

	float temp_cos, temp_sin;

	//rotate using pitch, and do rotation using the m_rotationPitch angle angle given from mouse input
		
	temp_cos = Cos(rotationPitch);
	temp_sin = Sin(rotationPitch);
		//position is set at (0, 0, 1) * m_rotationPitch, so rotate around X to get pitch
					
		//ROTATE X
	Set(m_position, (m_position[O_X]),
					(m_position[O_Y] * temp_cos) - (m_position[O_Z] * temp_sin),
					(m_position[O_Y] * temp_sin) + (m_position[O_Z] * temp_cos));

		//ROTATE Z (for funzies)
	/*Set(m_position, (m_position[O_X] * temp_cos) - (m_position[O_Y] * temp_sin),
					(m_position[O_X] * temp_sin) + (m_position[O_Y] * temp_cos),
					(m_position[O_Z]));*/

	//rotate the camera using the m_rotationY angle provided from mouse input
		//ROTATE Y
	temp_cos = Cos(m_rotationY);
	temp_sin = Sin(m_rotationY);
	Set(m_position, (m_position[O_Z] * temp_sin) + (m_position[O_X] * temp_cos),
					(m_position[O_Y]),
					(m_position[O_Z] * temp_cos) - (m_position[O_X] * temp_sin));
	
	
	AddTo(m_position, m_object->GetPosition());

	//set the lookAt vector
	Set(m_lookAt, 0.0f, lookAtHeight, 0.0f);
	AddTo(m_lookAt, m_object->GetPosition());

	//set the camera's forward vector
	Set(m_forward, m_position);
	Subtract(m_forward, m_object->GetPosition(), m_forward);

	//code for updating the forward movement vector relative to the camera (when the camera updates)
	Set(m_forwardMovement, this->m_forward);
	//TODO: Fix this code so that this points forward on to the plane that the object sits on
	Set(m_forwardMovement, m_forwardMovement[O_X], 0.0f, m_forwardMovement[O_Z]);
	Normalize(m_forwardMovement);

	gluLookAt(m_position[O_X], m_position[O_Y], m_position[O_Z],
				m_lookAt[O_X], m_lookAt[O_Y], m_lookAt[O_Z], 
				0.0f, 1.0f, 0.0f);
}

//Set the parameters used by the camera
void SphFollowCamera::SetParams(float followDistance, float cameraHeight, float lookAtHeight){
	m_followDistance = followDistance;
	m_cameraHeight = cameraHeight;
	m_lookAtHeight = lookAtHeight;
	m_targetHeight = cameraHeight;
}

//Set pitch inversion (up/down pitch of camera using mouse input)
void SphFollowCamera::SetInvertPitch(bool invertPitch){
	this->m_invertPitch = invertPitch;
}

//Return the vector that points "forward" for the camera
cfloat* SphFollowCamera::GetForward(){
	return this->m_forward;
}

//Return the camera's lookAt vector
cfloat* SphFollowCamera::GetLookAt(){
	return this->m_lookAt;
}

//Return the camera's Position vector
cfloat* SphFollowCamera::GetPosition(){
	return this->m_position;
}

//Return the vector that points "forward" for the object relative to the camera's position
cfloat* SphFollowCamera::GetForwardMovement(){
	return m_forwardMovement;
}

//Render the forward vector, originating from the m_object's position
void SphFollowCamera::RenderForwardMovement(){
	float forward[N_XYZ];

	Set(forward, this->GetForwardMovement());
	Multiply(forward, 1);
	AddTo(forward, m_object->GetPosition());

	RenderLine(m_object->GetPosition(), forward);
}