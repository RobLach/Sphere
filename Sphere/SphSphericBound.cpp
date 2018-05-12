#include "SphRenderer.h"
#include "SphSphericBound.h"

//Create SphSphericBound.
//object - which owns this bound.
//radius - of the bounding sphere.
SphSphericBound::SphSphericBound(SphObject* object, float radius) : SphPointBound(object) {
	this->m_radius = radius;
	this->m_boundType = Sphere;
}

//Render SphBoundSphere.
void SphSphericBound::Render(){
	//RenderSphere(this->m_radius);
}