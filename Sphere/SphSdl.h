#pragma once
#include "SphInclude.h"

struct SDL_Surface;

//SphSdl provides methods to setup and maintain SDL functionality.
class SphSdl {

private:
    SDL_Surface* m_screen;
    int m_screenWidth;
    int m_screenHeight;
    bool m_fullscreen;

public:
	SphSdl();
	int Init(bool fullscreen);
	int GetTicks();
	int GetScreenWidth();
	int GetScreenHeight();
	bool IsFullScreen();
	void SetWindowCaption(cchar* text);
	void SwapBuffers();
	void Quit();
};

extern SphSdl g_sdl;
