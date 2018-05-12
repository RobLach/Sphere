#include "SphMath.h"
#include "SphOpenGL.h"
#include "SphBound.h"
#include "SphRenderer.h"
#include "SphEnemyController.h"

cfloat l_moveSpeed = 5.0f;

// Create Enemy Controller
SphEnemyController::SphEnemyController(){

}
void SphEnemyController::Update(float seconds){
	float up[N_XYZ], forward[N_XYZ], newPos[N_XYZ], startingPos[N_XYZ], rotationAxis[N_XYZ], quat[N_QUAT];
	Set(up, 0.0f, 1.0f, 0.0f);
	Multiply(forward, l_moveSpeed * seconds);
	Set(startingPos, m_object->GetPosition());
	
	Add(newPos, startingPos, forward);
	this->m_object->SetPosition(newPos);

	AxisAngleToQuat(quat, rotationAxis, 0.5f); //0.5f Temp Hack lenght/radius?
	NormalizeQuat(quat);
	m_object->Rotate(quat);
	m_object->m_bound->UpdateBound();

}