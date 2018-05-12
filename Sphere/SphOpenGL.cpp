#include "SphOpenGL.h"
#include "SphSdl.h"
#include "SphMath.h"
#include "SphTgaImage.h"

SphOpenGL g_openGL;

cdouble SphOpenGL::s_orthoZNear = -1.0;
cdouble SphOpenGL::s_orthoZFar = 1.0;
cdouble SphOpenGL::s_perspZNear = 1.0;
cdouble SphOpenGL::s_perspZFar = 2400.0;
cdouble SphOpenGL::s_fieldOfView = 45.0;

//Create SphOpenGL.
SphOpenGL::SphOpenGL(){
}

//Initialize OpenGL.
//returns: 0 on success.
int SphOpenGL::Init(){
	m_screenWidth = g_sdl.GetScreenWidth();
	m_screenHeight = g_sdl.GetScreenHeight();
	SetUpPerspectiveMatrix();
	glViewport(0, 0, m_screenWidth, m_screenHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	GLfloat fogColor[4] = {181.0f/255.0f, 150.0f/255.0f, 105.0f/255.0f, 1.0f};	
	glFogi(GL_FOG_MODE, GL_LINEAR);		// Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);			// Set Fog Color
	glFogf(GL_FOG_DENSITY, 0.2f);				// How Dense Will The Fog Be
	glHint (GL_FOG_HINT, GL_FASTEST);
	glFogf(GL_FOG_START, 100.0f);				// Fog Start Depth
	glFogf(GL_FOG_END, 150.0f);				// Fog End Depth
	glEnable(GL_FOG);					// Enables GL_FOG


	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);
	//glTexEnvf(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);

	glewInit();
	return 0;
}

//Do things before actual rendering begins.
void SphOpenGL::PreRender(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetUpPerspectiveMatrix();
	glLoadIdentity();
}

//Do things after all rendering is done.
void SphOpenGL::PostRender(){

}

//Do things after everything for the frame is done.
void SphOpenGL::FinishRender(){
	glFlush();
	g_sdl.SwapBuffers();
	glFinish();
}

//Set up default orthogonal projection matrix based on the screen size.
void SphOpenGL::SetUpOrthoMatrix(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, m_screenWidth - 1, 0, m_screenHeight - 1, s_orthoZNear, s_orthoZFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Set up default perspective projection matrix based on the screen size.
void SphOpenGL::SetUpPerspectiveMatrix(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(s_fieldOfView, m_screenWidth / (float)m_screenHeight, s_perspZNear, s_perspZFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Check if an OpenGL error occured and raise a debug break if it had.
void SphOpenGL::CheckError(){
	if(SphDebug && glGetError() != GL_NO_ERROR){
		DebugBreak();
	}
}

//UnProject a point from window coordinates to world coordinates.
//x,y,z - point to project. z = 0.0 corresponds to near plane, 1.0 corresponds to far plane.
//result - will be set to the projected point.
void SphOpenGL::UnProject(int x, int y, float z, float* result){
	double modelViewMatrix[16], projectionMatrix[16];
	int viewport[4];
	double resultX, resultY, resultZ;
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluUnProject(x, m_screenHeight - y, z, modelViewMatrix, projectionMatrix, viewport, &resultX, &resultY, &resultZ);
	Set(result, resultX, resultY, resultZ);
}

//return - id of texture used to store screen color information.
uint SphOpenGL::CreateScreenColorTexture(){
	GLuint textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_screenWidth, m_screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	return textureID;
}

//return - id of texture used to store screen depth information.
uint SphOpenGL::CreateScreenDepthTexture(){
	GLuint textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, m_screenWidth, m_screenHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	return textureID;
}

//Load a TGA texture file.
//filePath - file path to the texture file.
//return - texture id.
uint SphOpenGL::LoadTexture(cchar* filePath){
	TgaImage tgaImage(filePath);
	if(tgaImage.m_data == NULL) return 0;
	uint textureID = 0;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLint components = (tgaImage.m_bytesPerPixel == 3) ? GL_RGB : GL_RGBA;
	gluBuild2DMipmaps(GL_TEXTURE_2D, components, tgaImage.m_width, tgaImage.m_height, components, GL_UNSIGNED_BYTE, tgaImage.m_data);

	return textureID;
}



//Load a font TGA texture file.
//filePath - file path to the tga file.
//textureSize - [N_XY] array will be set to [width, height] of the texture.
//return - texture id.
uint SphOpenGL::LoadFontTexture(cchar* filePath, int* textureSize){
	TgaImage tgaImage(filePath);
	textureSize[O_X] = tgaImage.m_width;
	textureSize[O_Y] = tgaImage.m_height;
	uint textureID = 0;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	GLint components = (tgaImage.m_bytesPerPixel == 3) ? GL_RGB : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, components, tgaImage.m_width, tgaImage.m_height, 0, components, GL_UNSIGNED_BYTE, tgaImage.m_data);

	return textureID;
}

//return - screen width.
int SphOpenGL::GetScreenWidth(){
	return m_screenWidth;
}

//return - screen height.
int SphOpenGL::GetScreenHeight(){
	return m_screenHeight;
}

//return - camera near
float SphOpenGL::GetNearClip(){
	//return (float)s_perspZNear;
	return (float)this->s_orthoZNear;
}

//return - camera far
float SphOpenGL::GetFarClip(){
	//return (float)s_perspZFar;
	return (float)this->s_orthoZFar;
}
