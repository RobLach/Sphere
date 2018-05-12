/*
SphSkybox.cpp - Created by Joe DiGiovanna (jdigiov2)
*/

#include "SphOpenGL.h"
#include "SphString.h"
#include "SphMath.h"
#include "SphSkybox.h"
#include "SphTgaImage.h"

//default constructor for initializing skybox
SphSkybox::SphSkybox(){
	this->m_distance = 0;
	this->m_locationX = 0;
	this->m_locationY = 0;
	this->m_locationZ = 0;
	this->m_hasColor = false;
}

//advanced constructor, letting you set starting centerpoint and apothem distance
SphSkybox::SphSkybox(float x, float y, float z, float dist){
	this->m_distance = dist;
	this->m_locationX = x;
	this->m_locationY = y;
	this->m_locationZ = z;
	this->m_hasColor = false;
}

//free up any data upon deletion
SphSkybox::~SphSkybox(){
	//delete this->m_texture;
}

//set appropriate textures 
void SphSkybox::setTextures(char* left, char* right, char* up, char* down, char* front, char* back){
	m_texture[0] = loadTexture(left);
	m_texture[1] = loadTexture(right);
	m_texture[2] = loadTexture(up);
	m_texture[3] = loadTexture(down);	
	m_texture[4] = loadTexture(front);
	m_texture[5] = loadTexture(back);
	m_textureCount = 6;
}

//set apothem distance
void SphSkybox::setRange(float distance){
	this->m_distance = distance;
}

//set vertex color and enable coloring
void SphSkybox::setVertexColor(float x, float y, float z){
	this->m_vertexColorX = x;
	this->m_vertexColorY = y;
	this->m_vertexColorZ = z;
	this->m_hasColor = true;
}

//render skybox
void SphSkybox::renderSkybox(){

	//see if we have a color we want to apply
	if (this->m_hasColor){
		glColor4f(this->m_vertexColorX, this->m_vertexColorY, this->m_vertexColorZ, 1.0f);
	}else{
		glColor4f(1.0, 1.0, 1.0, 1.0f);
	}

	// Save Current Matrix
	glPushMatrix();
 
	// Move the render space to the correct position
	glTranslatef(this->m_locationX, this->m_locationY, this->m_locationZ);
 
	// apply scale matrix
	glScalef(this->m_distance, this->m_distance, this->m_distance);

	float cz = -0.0f,cx = 1.0f;
	float r = 1.0f; 

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	// render top
	glBindTexture(GL_TEXTURE_2D,m_texture[2]);
	glBegin(GL_QUADS);	
		glTexCoord2f(cz, cz); glVertex3f( r, 1.0f, -r);
		glTexCoord2f(cx, cz); glVertex3f( r, 1.0f,  r);
		glTexCoord2f(cx, cx); glVertex3f(-r, 1.0f,  r); 
		glTexCoord2f(cz, cx); glVertex3f(-r, 1.0f, -r);
	glEnd();
 
	// render bottom
	glBindTexture(GL_TEXTURE_2D,m_texture[3]);
	glBegin(GL_QUADS);		
		glTexCoord2f(cz,cz);  glVertex3f(-r, -1.0f, -r);
		glTexCoord2f(cx,cz);  glVertex3f(-r, -1.0f,  r);
		glTexCoord2f(cx,cx);  glVertex3f( r, -1.0f,  r); 
		glTexCoord2f(cz,cx);  glVertex3f( r, -1.0f, -r);
	glEnd();
 
	// render left
	glBindTexture(GL_TEXTURE_2D,m_texture[0]);
	glBegin(GL_QUADS);		
		glTexCoord2f(cx,cx); glVertex3f(-1.0f,  r, -r);	
		glTexCoord2f(cz,cx); glVertex3f(-1.0f,  r,  r); 
		glTexCoord2f(cz,cz); glVertex3f(-1.0f, -r,  r);
		glTexCoord2f(cx,cz); glVertex3f(-1.0f, -r, -r);	
	glEnd();
 
	// render right
	glBindTexture(GL_TEXTURE_2D,m_texture[1]);
	glBegin(GL_QUADS);		
		glTexCoord2f(cz, cz); glVertex3f(1.0f, -r, -r);	
		glTexCoord2f(cx, cz); glVertex3f(1.0f, -r,  r); 
		glTexCoord2f(cx, cx); glVertex3f(1.0f,  r,  r);
		glTexCoord2f(cz, cx); glVertex3f(1.0f,  r, -r);
	glEnd();
 
	// render front
	glBindTexture(GL_TEXTURE_2D,m_texture[4]);
	glBegin(GL_QUADS);		
		glTexCoord2f(cx, cz); glVertex3f(-r, -r, 1.0f);
		glTexCoord2f(cx, cx); glVertex3f(-r,  r, 1.0f); 
		glTexCoord2f(cz, cx); glVertex3f( r,  r, 1.0f);
		glTexCoord2f(cz, cz); glVertex3f( r, -r, 1.0f);
	glEnd();
 
	// render back
	glBindTexture(GL_TEXTURE_2D,m_texture[5]);
	glBegin(GL_QUADS);		
		glTexCoord2f(cx, cz); glVertex3f( r, -r, -1.0f);
		glTexCoord2f(cx, cx); glVertex3f( r,  r, -1.0f); 
		glTexCoord2f(cz, cx); glVertex3f(-r,  r, -1.0f);
		glTexCoord2f(cz, cz); glVertex3f(-r, -r, -1.0f);
	glEnd();
 
	// Load Saved Matrix
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void SphSkybox::setLocation(float x, float y, float z){
	//update skybox location
	this->m_locationX = x;
	this->m_locationY = y;
	this->m_locationZ = z;
}

//load tga textures, needed to be different because of texture clamping and the use of linear mipmapping
uint SphSkybox::loadTexture(char* filePath){

	TgaImage tgaImage(filePath);
	if(tgaImage.m_data == NULL) return 0;
	uint textureID = 0;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);

	GLint components = (tgaImage.m_bytesPerPixel == 3) ? GL_RGB : GL_RGBA;
	gluBuild2DMipmaps(GL_TEXTURE_2D, components, tgaImage.m_width, tgaImage.m_height, components, GL_UNSIGNED_BYTE, tgaImage.m_data);

	return textureID;

}