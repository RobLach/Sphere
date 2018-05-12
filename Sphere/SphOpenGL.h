#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include "SphInclude.h"

//OpenGL provides methods to setup and maintain OpenGL functionality.
class SphOpenGL {

private:
	static cdouble s_orthoZNear;
	static cdouble s_orthoZFar;
	static cdouble s_perspZNear;
	static cdouble s_perspZFar;
	static cdouble s_fieldOfView;
	int m_screenWidth;
	int m_screenHeight;

public:
	SphOpenGL();
	int  Init();
	void PreRender();
	void PostRender();
	void FinishRender();
	void SetUpOrthoMatrix();
	void SetUpPerspectiveMatrix();
	void CheckError();
	void UnProject(int x, int y, float z, float* result);
	uint LoadTexture(cchar* filePath);
	uint LoadFontTexture(cchar* filePath, int* textureSize);
	uint CreateScreenColorTexture();
	uint CreateScreenDepthTexture();
	float prevProjectionMatrix[16];
	int  GetScreenWidth();
	int  GetScreenHeight();
	float GetFarClip();
	float GetNearClip();
};

extern SphOpenGL g_openGL;
