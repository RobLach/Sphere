#include "SphOpenGL.h"
#include "SphMath.h"
#include "SphDesertSkybox.h"

//Basic constructor for the desert Sphere Skybox
SphDesertSkybox::SphDesertSkybox(){
	m_skybox = new SphSkybox();
	m_skybox->setTextures("Textures/Skybox_Textures/LEFT.tga", "Textures/Skybox_Textures/RIGHT.tga", "Textures/Skybox_Textures/UP.tga", "Textures/Skybox_Textures/DOWN.tga", "Textures/Skybox_Textures/FRONT.tga", "Textures/Skybox_Textures/BACK.tga");
	m_skybox->setRange(2.0f);
	m_skybox->setLocation(0.0f, 1000.0f, 0.0f);
}

//Code required for updating each frame (based around the cameras location and a given distance)
void SphDesertSkybox::update(cfloat *pos, float range){
	m_skybox->setLocation(pos[0], pos[1], pos[2]);
	m_skybox->setRange(range);
}

//renders the actual skybox
void SphDesertSkybox::render(){
	glDisable(GL_FOG);
	m_skybox->renderSkybox();
	glEnable(GL_FOG);
}

