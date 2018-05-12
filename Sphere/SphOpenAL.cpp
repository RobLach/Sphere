#include "SphOpenAL.h"
#include "SphMath.h"

SphOpenAL g_openAL;

//Create new OpenAL.
SphOpenAL::SphOpenAL(){
}

//Initialize OpenAL system.
//return - 0 on success.
int SphOpenAL::Init(){
	m_device = alcOpenDevice(NULL);//"DirectSound Software"
	m_context = alcCreateContext(m_device, NULL);
	alcMakeContextCurrent(m_context);
	if(alGetError() != AL_NO_ERROR) return 1;

	cfloat defaultUp[] = {0.0f, 1.0f, 0.0f};
	SetListenerPosition(g_zeroes);
	SetListenerVelocity(g_zeroes);
	SetListenerOrientation(g_zeroes, defaultUp);
	return alGetError() != AL_NO_ERROR;
}

//Update OpenAL system.
void SphOpenAL::Update(){

	for(int n = 0; n < s_musicCount; n++){
		if(!m_music[n].IsActive()) continue;
		bool stillActive = m_music[n].Update();
		if(!stillActive) m_music[n].Free();
	}
}

//Find a music object that is not active.
//Note: You have to use the object immediately or it might become active.
//return - non-active music object. NULL if none found.
SphMusic* SphOpenAL::GetMusic(){
	for(int n = 0; n < s_musicCount; n++){
		if(!m_music[n].IsActive()){
			return &m_music[n];
		}
	}
	return NULL;
}

int SphOpenAL::getNumberMusicObjects()
{
	int numObjects = 0;
		for(int n = 0; n < s_musicCount; n++){
		if(m_music[n].IsActive()) numObjects++;
	}
		return numObjects;
}

//Set listener position.
void SphOpenAL::SetListenerPosition(cfloat* position){
	alListenerfv(AL_POSITION, position);
}

//Set listener velocity.
void SphOpenAL::SetListenerVelocity(cfloat* velocity){
	alListenerfv(AL_VELOCITY, velocity);
}

//Set listener direction and up vectors.
void SphOpenAL::SetListenerOrientation(cfloat* direction, cfloat* up){
	float orientation[2 * N_XYZ];
	Set(orientation, direction);
	Set(orientation + N_XYZ, up);
	alListenerfv(AL_ORIENTATION, orientation);
}

//Quit and free all associated resources.
void SphOpenAL::Quit(){
	for(int n = 0; n < s_musicCount; n++){
		if(!m_music[n].IsActive()) continue;
		m_music[n].Free();
	}

	alcMakeContextCurrent(NULL);
	alcDestroyContext(m_context);
	alcCloseDevice(m_device);
}
