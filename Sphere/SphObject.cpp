#include "SphOpenGL.h"
#include "SphRenderer.h"
#include "SphMath.h"
#include "SphOpenGL.h"
#include "SphObject.h"
#include "SphBound.h"
#include "SphCollisionDetection.h"

//Create SphObject.
SphObject::SphObject(){
	Set(this->m_position, g_zeroes);
	Set(this->m_velocity, g_zeroes);
	Set(this->m_netForce, g_zeroes);
	Set4(this->m_rotation, g_unitQuat);
	this->m_bound = NULL;
	this->m_health = 100;
	this->m_physicsTime = 0.0f;
}

//Apply object's matrix to openGL matrix.
void SphObject::ApplyMatrix(){
	glTranslatef(m_position[O_X], m_position[O_Y], m_position[O_Z]);
	if(MagnitudeSq(m_rotation + O_QX) > EPSILON){
		float angle = 2.0f * (float)acos(m_rotation[O_QW]);
		glRotatef(RadToDeg(angle), m_rotation[O_QX], m_rotation[O_QY], m_rotation[O_QZ]);
	}
}

//Render SphObject.
void SphObject::Render(){
	/*glPushMatrix();
	ApplyMatrix();
	RenderAxis();
	if(SphDebug && m_bound != NULL){
		m_bound->Render();
	}
	glPopMatrix();*/
}

//Update SphObject.
//seconds - since the last call.
void SphObject::Update(float seconds){
}

//Apply physics to this object.
void SphObject::ApplyPhysics(float seconds, float frictionFactor, float rotationFactor, bool gravity){
	const float physicsTick = 1 / 20.0f;
	bool didPhysics = false;
	m_physicsTime += seconds;
	while(m_physicsTime >= physicsTick){
		m_physicsTime -= physicsTick;
		didPhysics = true;
		cfloat gravityForce[N_XYZ] = {0.0f, -20.0f, 0.0f};

		//linear movement
		//update m_netForce
		if(gravity) AddTo(m_netForce, gravityForce);
		Multiply(m_netForce, physicsTick);

		//update velocity
		AddTo(m_velocity, m_netForce);
		Set(m_velocity, frictionFactor * m_velocity[O_X], m_velocity[O_Y], frictionFactor * m_velocity[O_Z]);

		//update position
		float positionDiff[N_XYZ];
		Set(positionDiff, m_velocity);
		Multiply(positionDiff, physicsTick);

		//update rotation
		float diffMagnXZ = sqrt(Sq(positionDiff[O_X]) + Sq(positionDiff[O_Z]));
		if(diffMagnXZ > BIG_EPSILON && rotationFactor > 0.0f){
			float rotAxis[N_XYZ], yAxis[N_XYZ];
			Set(yAxis, 0.0f, 1.0f, 0.0f);
			Cross(rotAxis, yAxis, positionDiff);
			Normalize(rotAxis);

			float rotationDiff[N_QUAT];
			Set(rotationDiff + O_QX, rotAxis);
			rotationDiff[O_QW] = 1.0f / (rotationFactor * diffMagnXZ);
			Normalize4(rotationDiff);
			RotateWorldSpace(rotationDiff);//also updates the bound, which is why it has to go first.
			Normalize4(m_rotation);
		}
		
		//do the translation and update the bound
		Translate(positionDiff);
	}

	if(didPhysics){
		//clear netforce/torque
		Set(m_netForce, g_zeroes);
	}
}

//vector - amount to translate SphObject by.
void SphObject::Translate(cfloat* vector, bool updateBound){
	if(m_bound != NULL && updateBound)m_bound->UpdateBound();
	AddTo(m_position, vector);
}

//quaternion - amount to rotate SphObject by.
void SphObject::Rotate(cfloat* quaternion){
	if(m_bound != NULL)m_bound->UpdateBound();
	QuatMultiplyBy(m_rotation, quaternion);
}

//quaternion - a quaternion in world space to rotate this object by
void SphObject::RotateWorldSpace(cfloat *worldSpaceQuaternion){
	float quat[N_QUAT];
	Set4(quat, worldSpaceQuaternion);
	QuatMultiplyBy(quat, m_rotation);
	Set4(m_rotation, quat);
	if(m_bound != NULL)m_bound->UpdateBound();
}

//x,y,z - amount to translate SphObject by.
void SphObject::Translate3(float x, float y, float z, bool updateBound){
	float vector[N_XYZ];
	Set(vector, x, y, z);
	Translate(vector, updateBound);
}

//position - SphObject's new position.
void SphObject::SetPosition(cfloat* position, bool updateBound){
	float diff[N_XYZ];
	Subtract(diff, position, m_position);
	Translate(diff);
	if(m_bound != NULL && updateBound)m_bound->UpdateBound();
}

//x,y,z - SphObject's new position.
void SphObject::SetPosition(float x, float y, float z, bool updateBound){
	float position[N_XYZ];
	Set(position, x, y, z);
	SetPosition(position, updateBound);
}

//quaternion - SphObject's new orientation.
void SphObject::SetRotation(cfloat* quaternion){
	Set4(m_rotation, quaternion);
	if(m_bound != NULL)m_bound->UpdateBound();
}

//return - object's position.
cfloat* SphObject::GetPosition(){
	return m_position;
}

//return - object's position.
cfloat* SphObject::GetRotation(){
	return m_rotation;
}

void SphObject::SetBound(SphBound* newBound) {
	m_bound = newBound;
}

//health - increase or decrease health.
void SphObject::ChangeHealth(int delta){
	m_health = Clamp(m_health + delta, 0, 100);
}

//health - increase or decrease health.
void SphObject::TakeDamage(int damage){
	m_health = Clamp(m_health - damage, 0, 100);
}

//health - return health value.
int SphObject::GetHealth(){
	return m_health;
}

//return - type of this object.
SphObject::SphObjectType SphObject::GetObjType(){
	return BASE_OBJ;
}

//force - to add to the object.
void SphObject::AddForce(cfloat* force){
	AddTo(m_netForce, force);
}

/***************************
*
* PHYSICS ATTRIBUTE GETTERS
*
****************************/

cfloat* SphObject::GetVelocity(){
	return m_velocity;
}

cfloat* SphObject::GetNetForce(){
	return m_netForce;
}

/***************************
*
* PHYSICS ATTRIBUTE SETTERS
*
****************************/

void SphObject::SetVelocity(cfloat* velocity){
	Set(m_velocity, velocity);
}

void SphObject::SetNetForce(cfloat* netForce){
	Set(m_netForce, netForce);
}