#include "SphSdlInput.h"
#include "SphOpenGL.h"
#include "SphMath.h"
#include "SphFreeCamera.h"

cfloat l_rotationSpeed = 0.004f;
cfloat l_moveSpeed = 5.0f;

//Create SphFreeCamera.
SphFreeCamera::SphFreeCamera(){
	Set(this->m_position, g_zeroes);
	this->m_rotationY = 0.0f;
	this->m_pitch = 0.0f;
}

//Apply camera settings to OpenGL matrix.
void SphFreeCamera::ApplyCamera(){
	float lookAt[N_XYZ];
	Set(lookAt, Cos(m_pitch), Sin(m_pitch), 0.0f);
	Set(lookAt, lookAt[O_X] * Sin(m_rotationY), lookAt[O_Y], lookAt[O_X] * Cos(m_rotationY));
	AddTo(lookAt, m_position);

	gluLookAt(m_position[O_X], m_position[O_Y], m_position[O_Z], lookAt[O_X], lookAt[O_Y], lookAt[O_Z], 0.0f, 1.0f, 0.0f);
}

//Update SphFreeCamera.
//seconds - since last call.
void SphFreeCamera::Update(float seconds){
	float forward[N_XYZ], right[N_XYZ];
	Set(forward, Sin(m_rotationY), 0.0f, Cos(m_rotationY));
	Multiply(forward, l_moveSpeed * seconds);
	Set(right, -forward[O_Z], 0.0f, forward[O_X]);

	if(g_sdlInput.IsShiftDown()){
		if(g_sdlInput.IsKeyDown(SDLK_SPACE)){
			m_position[O_Y] -= l_moveSpeed * seconds;
		}
	} else {
		if(g_sdlInput.IsKeyDown(SDLK_w)){
			AddTo(m_position, forward);
		}
		if(g_sdlInput.IsKeyDown(SDLK_s)){
			SubtractFrom(m_position, forward);
		}
		if(g_sdlInput.IsKeyDown(SDLK_d)){
			AddTo(m_position, right);
		}
		if(g_sdlInput.IsKeyDown(SDLK_a)){
			SubtractFrom(m_position, right);
		}
		if(g_sdlInput.IsKeyDown(SDLK_SPACE)){
			m_position[O_Y] += l_moveSpeed * seconds;
		}
	}

	if(g_sdlInput.IsMouseButtonDown(g_sdlInput.s_rightMouseButton)){
		m_rotationY -= l_rotationSpeed * g_sdlInput.GetMouseDX();
		m_pitch -= l_rotationSpeed * g_sdlInput.GetMouseDY();
		m_pitch = Clamp(m_pitch, -PId2 + BIG_EPSILON, PId2 - BIG_EPSILON);
	}
}

//return - forward vector for this camera.
cfloat* SphFreeCamera::GetForward(){
	return g_zeroes;//we shouldn't really need this.
}