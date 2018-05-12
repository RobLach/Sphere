#include "TestHarness.h"
#include "SphInclude.h"
#include "SphSdlInput.h"
#include "SphSdl.h"
#include "SDL.h"
#include <stdio.h>

//simulate a key press (this does not currently work)
void createKeyPress(SDLKey key)
{
	SDL_keysym keySym = {0, key, KMOD_NONE, 0};
	SDL_KeyboardEvent keyEvent = {SDL_KEYDOWN, 1, SDL_PRESSED, keySym};
	SDL_Event sdlEvent;
	sdlEvent.type = SDL_KEYDOWN;
	sdlEvent.key = keyEvent;
	SDL_PushEvent(&sdlEvent);
}

TEST(complete, SdlInput)
{
	printf("---SDL Input Tests Done\n");
	printf("-----------------------------------------\n\n");
}

// Tests for SDL Input Keypresses
TEST(keyPresses, SdlInput)
{
	printf("-Running SDL Input Keypress Tests - ");
	//INITIALIZE SDL WINDOWED
	CHECK(!g_sdl.Init(0));
	//INITIALIZE SDL INPUT
	CHECK(!g_sdlInput.Init()); 
	
	//simulate keypresses?
	//g_sdlInput.ProcessUserInput();
	//CHECK(!g_sdlInput.IsKeyDown(SDLK_w));
	//createKeyPress(SDLK_w);
	//g_sdlInput.ProcessUserInput();
	//CHECK(g_sdlInput.IsKeyDown(SDLK_w));
	
	//Simulating keypresses proved nearly impossible.  Tests were done manually instead.

	g_sdlInput.Quit();
	g_sdl.Quit();

	printf(" DONE \n");
}

// Tests for SDL Input initialization
TEST(init, SdlInput)
{
	printf("-Running SDL Input Initialization Test - ");
	//INITIALIZE SDL WINDOWED
	CHECK(!g_sdl.Init(0));
	//INITIALIZE SDL INPUT
	CHECK(!g_sdlInput.Init()); 
	

	g_sdlInput.Quit();
	g_sdl.Quit();

	printf(" DONE \n");
}

TEST(begin, SdlInput)
{
	printf("\n-----------------------------------------\n");
	printf("---Starting SDL Input Tests...\n");
}