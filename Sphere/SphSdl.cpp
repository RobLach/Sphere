#include "SDL.h"
#include "SphSdl.h"

SphSdl g_sdl;

//Create SphSDL object.
SphSdl::SphSdl(){
    this->m_screen = NULL;
}

//Initialize SDL and setup the window or full screen mode.
//fullscreen - true if fullscreen mode should be on.
//returns: 0 on success.
int SphSdl::Init(bool fullscreen){
	m_fullscreen = fullscreen;
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) == -1) return 1;
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	//SDL_GL_STENCIL_SIZE

	int flags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL;
	if(m_fullscreen) flags |= SDL_FULLSCREEN;

	if(fullscreen){
		SDL_Rect** modes = SDL_ListModes(NULL, flags);
		m_screenWidth = modes[0]->w;
		m_screenHeight = modes[0]->h;
	} else {
		m_screenWidth = 800;
		m_screenHeight = 600;
	}

	m_screen = SDL_SetVideoMode(m_screenWidth, m_screenHeight, 32, flags);
	return (m_screen == NULL) ? 1 : 0;
}

//Return number of ticks since the start of the program.
int SphSdl::GetTicks(){
	return SDL_GetTicks();
}

//return - current screen width.
int SphSdl::GetScreenWidth(){
	return m_screenWidth;
}

//return - current screen height.
int SphSdl::GetScreenHeight(){
	return m_screenHeight;
}

//return - true if the game is in fullscreen mode.
bool SphSdl::IsFullScreen(){
	return m_fullscreen;
}

//Set the window caption.
void SphSdl::SetWindowCaption(cchar* text){
	SDL_WM_SetCaption(text, NULL);
}

//Swap buffers.
void SphSdl::SwapBuffers(){
	SDL_GL_SwapBuffers();
}

//Quit and free all resources.
void SphSdl::Quit(){
	SDL_Quit();
}
