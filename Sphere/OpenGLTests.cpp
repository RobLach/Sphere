#include "TestHarness.h"
#include "SphInclude.h"
#include "SphOpenGL.h"
#include "SphSdl.h"
#include <stdio.h>

int check_openGL_error()
{
	//CHECK FOR OPENGL ERRORS
	GLenum errCode;
	const GLubyte* errString;
	if((errCode = glGetError()) != GL_NO_ERROR)
	{
		errString = gluErrorString(errCode);
		return (int)errCode;
	}
	else
	{
		return 0;
	}

	//HAVE IT RETURN ERROR STRING LATER
}

TEST(complete, OpenGL)
{
	printf("---OpenGL Tests Done\n");
	printf("-----------------------------------------\n\n");
}

// Tests for OpenGL rendering
TEST(render, OpenGL)
{
	printf("-Running OpenGL Rendering Test - ");

	//INITIALIZE SDL WINDOW FIRST
	CHECK(!g_sdl.Init(0));
	//INITIALIZE OPENGL
	CHECK(!g_openGL.Init());

	//PRE-RENDER
	g_openGL.PreRender();
	//CHECK FOR OPENGL ERRORS
	CHECK(!check_openGL_error());

	//POST-RENDER
	g_openGL.PostRender();
	//CHECK FOR OPENGL ERRORS
	CHECK(!check_openGL_error());

	//FINISH RENDER
	g_openGL.FinishRender();
	//CHECK FOR OPENGL ERRORS
	CHECK(!check_openGL_error());


	g_sdl.Quit();
	printf(" DONE \n");
}

TEST(setup_OrthoPerspective_Matrices, OpenGL)
{
	printf("-Running OpenGL Orthogonal and Perspective Matrix Tests - ");

	//INITIALIZE SDL WINDOW and OPENGL
	CHECK(!g_sdl.Init(0));
	CHECK(!g_openGL.Init());
	
	//ACTUAL FUNCTIONS
	g_openGL.SetUpOrthoMatrix();
	g_openGL.SetUpPerspectiveMatrix();

	//CHECK FOR OPENGL ERRORS
	CHECK(!check_openGL_error());

	g_sdl.Quit();
	printf(" DONE \n");
}

// Tests for OpenGL initialization
TEST(init, OpenGL)
{
	printf("-Running OpenGL Initialization Test - ");

	//INITIALIZE SDL WINDOW FIRST
	CHECK(!g_sdl.Init(0));

	//INITIALIZE OPENGL
	CHECK(!g_openGL.Init());

	//CHECK FOR OPENGL ERRORS
	CHECK(!check_openGL_error());

	//make sure width and height are set
	CHECK(g_openGL.GetScreenWidth() != 0);
	CHECK(g_openGL.GetScreenHeight() != 0);

	g_sdl.Quit();
	printf(" DONE \n");
}

TEST(begin, OpenGL)
{
	printf("\n-----------------------------------------\n");
	printf("---Starting OpenGL Tests...\n");
}