#pragma once
#include "alc.h"
#include "SphInclude.h"
#include "SphMusic.h"

//OpenAL provides methods to setup and maintain OpenAL functionality.
//OpenAL contains and manages all playing music/sounds.
class SphOpenAL {

private:
	static cint s_musicCount = 8;

	ALCdevice* m_device;
	ALCcontext* m_context;
	SphMusic m_music[s_musicCount];

public:
	SphOpenAL();
	int Init();
	void Update();
	SphMusic* GetMusic();

	void SetListenerPosition(cfloat* position);
	void SetListenerVelocity(cfloat* velocity);
	void SetListenerOrientation(cfloat* direction, cfloat* up);
	int getNumberMusicObjects();
	void Quit();
};

extern SphOpenAL g_openAL;
