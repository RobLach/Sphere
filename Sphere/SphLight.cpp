#include "SphMath.h"
#include "SphLight.h"

//Create SphLight.
SphLight::SphLight(){
	// Create light components
	Set(ambientLight, 0.75f, 0.75f, 0.75f, 1.0f);
	Set(diffuseLight, 1.0f, 1.0f, 1.0f, 1.0f);
	Set(specularLight, 1.0f, 1.0f, 1.0f, 1.0f);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
}

//Render SphLight.
void SphLight::Render(){
	// Assign created components to GL_LIGHT0
	Set(lightPosition, this->GetPosition()[O_X], this->GetPosition()[O_Y], this->GetPosition()[O_Z], 0.0f);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

//Update SphLight.
void SphLight::Update(float seconds)
{
}