#pragma once
#include "SDL_keysym.h"
#include "SphInclude.h"

//SDLInput provides methods to setup and maintain SDL mouse and keyboard input functionality.
class SdlInput {

private:

	byte* m_keystates;
	byte* m_oldKeystates;
	int m_mouseX, m_mouseY;
	int m_mouseDX, m_mouseDY;
	byte m_mouseButtons;
	byte m_mouseButtonsDP;
	byte m_oldMouseButtons;
	int m_keyCount;
	bool m_mouseActive;

	char AdjustForShift(char c);

public:

	static cint s_leftMouseButton = 1;
	static cint s_middleMouseButton = 2;
	static cint s_rightMouseButton = 3;

	SdlInput();
	int Init();
	void ProcessUserInput();
	bool IsKeyDown(int key);
	bool KeyDownEvent(int key);
	bool KeyUpEvent(int key);
	bool IsMouseButtonDown(int button);
	bool IsMouseButtonTriggered(int button);
	bool MouseButtonDownEvent(int button);
	bool MouseButtonUpEvent(int button);
	void MoveMouseTo(int x, int y);
	bool IsShiftDown();
	bool IsCtrlDown();
	bool IsAltDown();
	bool IsMouseActive();
	bool SwitchMouseControl();
	void MouseControlOn();
	void MouseControlOff();
	char GetPrintableKeyDownEvent();
	int GetMouseX();
	int GetMouseY();
	int GetMouseDX();
	int GetMouseDY();
	void Quit();
};

extern SdlInput g_sdlInput;
