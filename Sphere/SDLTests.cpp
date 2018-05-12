#include "TestHarness.h"
#include "SphInclude.h"
#include "SphSdl.h"
#include <stdio.h>

TEST(complete, SDL)
{
	printf("---SDL Tests Done\n");
	printf("-----------------------------------------\n\n");
}


// Tests for SDL fullscreen initialization
TEST(fullscreenInit, SDL)
{
	printf("-Running SDL FullScreen Initialization Test - ");
	//TRY INITIALIZE
	CHECK(!g_sdl.Init(1));
	
	//MAKE SURE VARIABLES ARE SET
	CHECK(g_sdl.IsFullScreen());
	CHECK(g_sdl.GetScreenHeight() > 0);
	CHECK(g_sdl.GetScreenWidth() > 0);

	g_sdl.Quit();
	printf(" DONE \n");
}

// Tests for SDL windowed initialization
TEST(windowedInit, SDL)
{
	printf("-Running SDL Windowed Initialization Test - ");
	//TRY INITIALIZE
	CHECK(!g_sdl.Init(0));

	//MAKE SURE VARIABLES ARE CORRECTLY SET
	CHECK(!g_sdl.IsFullScreen());
	CHECK(g_sdl.GetScreenHeight() > 0);
	CHECK(g_sdl.GetScreenWidth() > 0);

	g_sdl.Quit();
	printf(" DONE \n");
}

TEST(begin, SDL)
{
	printf("\n-----------------------------------------\n");
	printf("---Starting SDL Tests...\n");
}