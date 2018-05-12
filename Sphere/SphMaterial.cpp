#include <map>
#include <string>
#include "SphOpenGL.h"
#include "SphMaterial.h"
#include "SphMath.h"
using namespace std;

//Use a map to remember what texture we already loaded.
typedef map<string, int> SphTexturesDict;
typedef pair<string, int> SphTexturesPair;
static SphTexturesDict l_textures;

//Create SphMaterial.
SphMaterial::SphMaterial(){
	Set4(this->m_diffuse, g_ones);
	Set4(this->m_specular, g_zeroes);
	this->m_shininess = 1.0f;
	this->m_texture = 0;
}

//file - to load material info from.
void SphMaterial::Load(FILE* file){
	char shaderName[256];
	fscanf(file, "a %f %f %f\n", m_diffuse + O_R, m_diffuse + O_G, m_diffuse + O_B);
	fscanf(file, "d %f %f %f\n", m_diffuse + O_R, m_diffuse + O_G, m_diffuse + O_B);
	fscanf(file, "s %f %f %f\n", m_specular + O_R, m_specular + O_G, m_specular + O_B);
	fscanf(file, "h %f\n", &m_shininess);
	fscanf(file, "shader %s\n", shaderName);
}

//Load texture from a file.
//filePath - to the texture file.
void SphMaterial::LoadTexture(cchar* filePath){
	if(filePath == NULL){
		m_texture = 0;
		return;
	}

	string key(filePath);
	SphTexturesDict::iterator iter = l_textures.find(key);
	if(iter == l_textures.end()){
		m_texture = g_openGL.LoadTexture(filePath);
		l_textures.insert(SphTexturesPair(key, m_texture));
	} else {
		m_texture = iter->second;
	}
}

//Use this material to set OpenGL parameters.
void SphMaterial::ActivateMaterial(){
	float ambient[N_RGBA];
	Set4(ambient, m_diffuse);
	Multiply(ambient, 0.5f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}
