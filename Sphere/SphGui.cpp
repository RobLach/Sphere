#include "SphOpenGL.h"
#include "SphGui.h"
#include "SphGuiObject.h"
#include "SphGuiText.h"
#include "SphGuiTextBox.h"

SphGui g_gui;

//Create SphGui.
SphGui::SphGui(){
}

//Init SphGui.
//return - 0 on success.
int SphGui::Init(){
	return 0;
}

//Render SphGui.
void SphGui::Render(){
	glDepthFunc(GL_ALWAYS);
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glPushMatrix();
	g_openGL.SetUpOrthoMatrix();
	glTranslatef(0.0f, (float)g_openGL.GetScreenHeight(), 0.0f);
	glScalef((float)g_openGL.GetScreenWidth(), -(float)g_openGL.GetScreenHeight(), 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for(SphGuiObjectList::iterator iter = m_objects.begin(); iter != m_objects.end(); iter++){
		(*iter)->Render();
	}

	glDisable(GL_BLEND);
	glPopMatrix();
	glEnable(GL_FOG);
	glEnable(GL_LIGHTING);
	glDepthFunc(GL_LEQUAL);
}

//Update SphGui.
//seconds - since last call.
void SphGui::Update(float seconds){
	for(SphGuiObjectList::iterator iter = m_objects.begin(); iter != m_objects.end(); iter++){
		(*iter)->Update(seconds);
	}
}

//object - to add to the gui.
void SphGui::AddGuiObject(SphGuiObject* object){
	m_objects.push_front(object);
}

//object - to remove from the gui.
void SphGui::RemoveGuiObject(SphGuiObject* object){
	m_objects.remove(object);
}

//Quit SphGui and free all associated resources.
void SphGui::Quit(){
	m_objects.clear();
}
