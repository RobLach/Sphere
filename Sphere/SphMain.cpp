#include "SphSdl.h"
#include "SphSdlInput.h"
#include "SphOpenGL.h"
#include "SphTextRenderer.h"
#include "SphOpenAL.h"
#include "SphMath.h"
#include "SphGui.h"
#include "SphGame.h"
#include "SphpAPI.h"
#include "TestHarness.h"
#include <conio.h>


//Initialize all necessary systems.
//fullscreen - true if fullscreen mode should be established.
//return - 0 on success.
int Init(bool fullscreen){
	if(g_sdl.Init(fullscreen)) return 1;
	if(g_sdlInput.Init()) return 1;
	if(g_openGL.Init()) return 1;
	if(g_openAL.Init()) return 1;
	if(g_textRenderer.Init()) return 1;
	if(g_gui.Init()) return 1;
	if(g_game.Init()) return 1;

	g_sdl.SetWindowCaption("Sphere");
	srand(g_sdl.GetTicks());
	return 0;
}

//Quit all subsystems.
void Quit(){
	g_game.Quit();
	g_gui.Quit();
	g_openAL.Quit();
	g_sdlInput.Quit();
	g_sdl.Quit();
}

//Start Unit Testing
int UnitTesting(){
	printf("-=Running Unit Tests=-\n");
	TestResult tr;
	TestRegistry::runAllTests(tr);
	//Quit();	Causes crashes since all the objects inside this don't really exist anymore.
	_getch();
	exit(0);
}


//Entry point into the game.
//argc - number of parameters passed to the program.
//args - array of strings containing the parameters.
//return - 0 if program ran successfully.
int main(int argc, char* args[]) {
	//Default program argumetns = {exe path, fullscreen}
	char* defaultArgsDeb[] = { args[0], "0" };
	char* defaultArgsRel[] = { args[0], "0" };
	cint argCount = sizeof(defaultArgsDeb) / sizeof(char*);
	if(argc != argCount){
		argc = argCount;
		args = SphDebug ? defaultArgsDeb : defaultArgsRel;
	}
	
	//Cheap hack since i'm tired - rob
	int argType = atoi(args[1]);
	if(argType == 2){
		UnitTesting();
	} else {
		if(Init(atoi(args[1]) != 0)) return 1;
	}

	//FPS variables
	cint fpsCount = 20;
	int fpsIndex = 0;
	float fpsBuffer[fpsCount];
	float fpsTotal = 0.0f;
	SetN(fpsBuffer, 0.0f, fpsCount);
	char fpsStringBuffer[512];

	int oldTicks = g_sdl.GetTicks();
	int ticks = g_sdl.GetTicks();

	//Main game loop
	while(!g_game.m_exit){
		oldTicks = ticks;
		ticks = g_sdl.GetTicks();
		float seconds = Min(0.05f, (ticks - oldTicks) / 1000.0f);
		if(seconds <= 0.0f) continue;

		g_openGL.PreRender();
		g_game.Render();
		g_gui.Render();
		g_openGL.PostRender();

		g_openAL.Update();
		g_sdlInput.ProcessUserInput();
		g_gui.Update(seconds);
		g_game.Update(seconds);

		g_openGL.FinishRender();

		if(SphDebug && !g_sdl.IsFullScreen()){
			fpsTotal -= fpsBuffer[fpsIndex];
			fpsBuffer[fpsIndex] = 1.0f / seconds;
			fpsTotal += fpsBuffer[fpsIndex];
			fpsIndex = (fpsIndex + 1) % fpsCount;
			sprintf(fpsStringBuffer, "FPS: %f", fpsTotal / fpsCount);
			g_sdl.SetWindowCaption(fpsStringBuffer);
		}
	}

	Quit();
	return 0;
}

