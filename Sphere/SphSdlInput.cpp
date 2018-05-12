#include "SDL.h"
#include "SphSdlInput.h"
#include "SphSdl.h"

SdlInput g_sdlInput;

//Create SdlInput.
SdlInput::SdlInput(){
    this->m_keystates = NULL;
    this->m_oldKeystates = NULL;
    this->m_mouseX = this->m_mouseY = 0;
    this->m_mouseDX = this->m_mouseDY = 0;
    this->m_mouseButtons = 0;
    this->m_mouseButtonsDP = 0;
    this->m_oldMouseButtons = 0;
    this->m_keyCount = 0;
	this->m_mouseActive = false;
}

//Initialize SDL input.
//return - 0 on success.
int SdlInput::Init(){
	m_keystates = SDL_GetKeyState(&m_keyCount);
	m_oldKeystates = new byte[m_keyCount];
	if(g_sdl.IsFullScreen()){
		SDL_WM_GrabInput(SDL_GRAB_ON);
	}
	SDL_ShowCursor(SDL_DISABLE);
	return 0;
}

//Process user input.
void SdlInput::ProcessUserInput() {
	memcpy(m_oldKeystates, m_keystates, m_keyCount * sizeof(byte));

	SDL_PumpEvents();
	SDL_GetMouseState(&m_mouseX, &m_mouseY);
	m_oldMouseButtons = m_mouseButtons;
	m_mouseButtons = SDL_GetRelativeMouseState(&m_mouseDX, &m_mouseDY);
	m_mouseButtonsDP = m_mouseButtons ^ m_oldMouseButtons;
}

//return - true if the given key is pressed.
bool SdlInput::IsKeyDown(int key){
	return m_keystates[key] != 0;
}

//return - true if shift key is pressed.
bool SdlInput::IsShiftDown(){
	return IsKeyDown(SDLK_RSHIFT) || IsKeyDown(SDLK_LSHIFT);
}

//return - true if ctrl key is pressed.
bool SdlInput::IsCtrlDown(){
	return IsKeyDown(SDLK_RCTRL) || IsKeyDown(SDLK_LCTRL);
}

//return - true if alt key is pressed.
bool SdlInput::IsAltDown(){
	return IsKeyDown(SDLK_RALT) || IsKeyDown(SDLK_LALT);
}

//return - true if mouse is active, false if camera is active
bool SdlInput::IsMouseActive(){
	return m_mouseActive;
}

//return - true if mouse is set to active, false is camera is set to active
bool SdlInput::SwitchMouseControl(){
	if(!m_mouseActive){
		MouseControlOn();
	}
	else{
		MouseControlOff();
	}
	return m_mouseActive;
}

//set mouse to active
void SdlInput::MouseControlOn(){
	SDL_ShowCursor(SDL_ENABLE);
	m_mouseActive = true;
}

//set mouse to inactive
void SdlInput::MouseControlOff(){
	SDL_ShowCursor(SDL_DISABLE);
	MoveMouseTo(g_sdl.GetScreenWidth()/2, g_sdl.GetScreenHeight()/2);
	m_mouseActive = false;
}

//return - a printable character pressed down this frame. '\0' if none.
char SdlInput::GetPrintableKeyDownEvent(){
	for(int key = 'a'; key <= 'z'; key++){
		if(KeyDownEvent(key)) return AdjustForShift((char)key);
	}
	for(int key = ' '; key <= '@'; key++){
		if(KeyDownEvent(key)) return AdjustForShift((char)key);
	}
	for(int key = '['; key <= '`'; key++){
		if(KeyDownEvent(key)) return AdjustForShift((char)key);
	}
	//for(int key = '{'; key <= '~'; key++){
		//if(KeyDownEvent(key)) return AdjustForShift((char)key);
	//}
	return '\0';
}

//return - true if the key was pressed this frame.
bool SdlInput::KeyDownEvent(int key){
	return IsKeyDown(key) && m_keystates[key] != m_oldKeystates[key];
}

//return - true if the key was released this frame.
bool SdlInput::KeyUpEvent(int key){
	return !IsKeyDown(key) && m_keystates[key] != m_oldKeystates[key];
}

//return - true if the given mouse button is pressed.
bool SdlInput::IsMouseButtonDown(int button){
	return (m_mouseButtons & SDL_BUTTON(button)) != 0;
}

//return - true if the given mouse button was triggered this frame.
bool SdlInput::IsMouseButtonTriggered(int button){
	return (m_mouseButtonsDP & SDL_BUTTON(button)) != 0;
}

//return - true if the given mouse button was pressed this frame.
bool SdlInput::MouseButtonDownEvent(int button){
	return IsMouseButtonDown(button) && IsMouseButtonTriggered(button);
}

//return - true if the given mouse button was released this frame.
bool SdlInput::MouseButtonUpEvent(int button){
	return !IsMouseButtonDown(button) && IsMouseButtonTriggered(button);
}

//return - mouse x coordinate
int SdlInput::GetMouseX(){
	return m_mouseX;
}

//return - mouse y coordinate
int SdlInput::GetMouseY(){
	return m_mouseY;
}

//return - mouse x displacement
int SdlInput::GetMouseDX(){
	return m_mouseDX;
}

//return - mouse y displacement
int SdlInput::GetMouseDY(){
	return m_mouseDY;
}

// Move Mouse to designated X and Y
void SdlInput::MoveMouseTo(int x, int y)
{
	SDL_WarpMouse(x, y);
}

//Quit and free all resources.
void SdlInput::Quit(){
	DELETE_ARRAY(m_oldKeystates);
}

// c - character to adjust.
//return - final character value, adjusted if shift is pressed.
char SdlInput::AdjustForShift(char c){
	if(!IsShiftDown()) return c;
	if('a' <= c && c <= 'z') return c - 'a' + 'A';
	switch(c){
		case '`': return '~';
		case '1': return '!';
		case '2': return '@';
		case '3': return '#';
		case '4': return '$';
		case '5': return '%';
		case '6': return '^';
		case '7': return '&';
		case '8': return '*';
		case '9': return '(';
		case '0': return ')';
		case '-': return '_';
		case '=': return '+';
		case '[': return '{';
		case ']': return '}';
		case '\\': return '|';
		case ';': return ':';
		case '\'': return '"';
		case ',': return '<';
		case '.': return '>';
		case '/': return '?';
			

	}
	return c;
}
