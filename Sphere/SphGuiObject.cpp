#include "SphMath.h"
#include "SphRenderer.h"
#include "SphGuiObject.h"

//Create SphGuiObject.
SphGuiObject::SphGuiObject(){
	Set(this->m_left, g_zeroes);
	Set(this->m_right, g_zeroes);
	Set4(this->m_frameColor, g_ones);
	this->m_frameLineWidth = 1.0f;
	this->m_drawFrame = false;
}

//Render SphGuiObject.
void SphGuiObject::Render(){
	if(m_drawFrame){
		RenderGuiFrame(m_frameColor, m_left, m_right, m_frameLineWidth);
	}
}

//Update SphGuiObject.
//seconds - since last call.
void SphGuiObject::Update(float seconds){
}

//Destroy SphGuiObject.
SphGuiObject::~SphGuiObject(){
}
