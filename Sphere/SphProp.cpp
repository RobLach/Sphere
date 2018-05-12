#include "SphOpenGL.h"
#include "SphProp.h"
#include "SphBound.h"

//Create SphProp.
SphProp::SphProp(SphMesh* mesh){
	m_mesh = mesh;
	m_bounceFactor = 1.0f;
	m_frictionFactor = 0.93f;
	m_magnet = false;
}

//Render SphProp.
void SphProp::Render(){
	glPushMatrix();
	ApplyMatrix();
	m_mesh->Render();
	glPopMatrix();
}

//mesh - to use for this prop.
void SphProp::SetMesh(SphMesh* mesh){
	m_mesh = mesh;
}

//return - object's type.
SphObject::SphObjectType SphProp::GetObjType(){
	return PROP_OBJ;
}

//return - bounce factor for this surface.
float SphProp::GetBounceFactor(){
	return m_bounceFactor;
}

//return - friction factor for this surface.
float SphProp::GetFrictionFactor(){
	return m_frictionFactor;
}

//return - true if this surface is a magnet.
bool SphProp::IsMagnet(){
	return m_magnet;
}