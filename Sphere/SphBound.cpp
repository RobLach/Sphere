#include "SphBound.h"

//Create SphBound.
//object - which owns this bound.
SphBound::SphBound(SphObject* object){
	this->m_object = object;
	this->m_boundType = None;
}

//return - object which owns this bound.
SphObject* SphBound::GetObject(){
	return m_object;
}

//return - type of this bound.
SphBound::BoundType SphBound::GetBoundType(){
	return m_boundType;
}