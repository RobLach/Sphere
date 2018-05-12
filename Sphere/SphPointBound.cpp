#include "SphMath.h"
#include "SphRenderer.h"
#include "SphPointBound.h"
#include "SphObject.h"

//Create SphPointBound.
//object - which owns this bound.
SphPointBound::SphPointBound(SphObject* object) : SphBound(object) {
	this->m_boundType = Point;
	Set(this->m_oldPosition, g_zeroes);
}

//Render SphPointBound.
void SphPointBound::Render(){
	float offset[N_XYZ];
	Subtract(offset, m_oldPosition, m_object->GetPosition());
	RenderLine(g_zeroes, offset);
}

//Update this bound to reflect the current state of the object.
void SphPointBound::UpdateBound(){
	Set(m_oldPosition, m_object->GetPosition());
}

//return - bound's position last frame.
cfloat* SphPointBound::GetOldPosition(){
	return m_oldPosition;
}
