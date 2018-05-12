#include "SphMath.h"
#include "SphMusic.h"

//Create SphMusic.
SphMusic::SphMusic(){
	this->m_isActive = false;
}

//Open a music file.
//filePath - file path to the OGG file.
//Returns 0 if success
int SphMusic::Open(cchar* filePath){
	if(ov_fopen((char*)filePath, &m_oggStream) < 0) return 1;

	vorbis_info* vorbisInfo = ov_info(&m_oggStream, -1);
	m_format = (vorbisInfo->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
	m_rate = vorbisInfo->rate;

	alGenBuffers(N_PAIR, m_buffers);
	alGenSources(1, &m_source);

	alSourcei(m_source, AL_SOURCE_RELATIVE, AL_FALSE);
	SetRolloff(0.0f);
	SetPosition(g_zeroes);
	SetVelocity(g_zeroes);
	SetDirection(g_zeroes);
	return 0;
}

// Will open a .ogg audio file and play it.
int SphMusic::OpenAndPlay(cchar* filePath){
	if(ov_fopen((char*)filePath, &m_oggStream) < 0) return 1;

	vorbis_info* vorbisInfo = ov_info(&m_oggStream, -1);
	m_format = (vorbisInfo->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
	m_rate = vorbisInfo->rate;

	alGenBuffers(N_PAIR, m_buffers);
	alGenSources(1, &m_source);

	alSourcei(m_source, AL_SOURCE_RELATIVE, AL_FALSE);
	SetRolloff(0.0f);
	SetPosition(g_zeroes);
	SetVelocity(g_zeroes);
	SetDirection(g_zeroes);
	if(StartPlaying(false))
	{
		return 0;
		Update();
	}
	return 1;
}

// Will open a .ogg file and play it. The audio will loop if loopForever is true.
int SphMusic::OpenAndPlay(cchar* filePath, float gain, bool loopForever){
	if(ov_fopen((char*)filePath, &m_oggStream) < 0) return 1;

	vorbis_info* vorbisInfo = ov_info(&m_oggStream, -1);
	m_format = (vorbisInfo->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
	m_rate = vorbisInfo->rate;

	alGenBuffers(N_PAIR, m_buffers);
	alGenSources(1, &m_source);

	alSourcei(m_source, AL_SOURCE_RELATIVE, AL_FALSE);
	SetRolloff(0.0f);
	SetPosition(g_zeroes);
	SetVelocity(g_zeroes);
	SetDirection(g_zeroes);
	SetVolume(gain);
	if(StartPlaying(loopForever))
	{
		return 0;
		Update();
	}
	return 1;
}

//Set the position of the music m_source in global coordinates.
void SphMusic::SetPosition(cfloat* position){
	alSourcefv(m_source, AL_POSITION, position);
}

//Set the velocity of the music m_source.
void SphMusic::SetVelocity(cfloat* velocity){
	alSourcefv(m_source, AL_VELOCITY, velocity);
}

//Set the direction of the music m_source.
void SphMusic::SetDirection(cfloat* direction){
	alSourcefv(m_source, AL_DIRECTION, direction);
}

//Set the rolloff factor for the music m_source.
void SphMusic::SetRolloff(cfloat rolloff){
	alSourcef(m_source, AL_ROLLOFF_FACTOR, rolloff);
}

//Sets the volume factor
void SphMusic::SetVolume(cfloat gain){
	alSourcef(m_source, AL_GAIN, gain);
}

//Start playing the music.
//doLoop - true if the music will loop forever.
//return - true if it's playing.
bool SphMusic::StartPlaying(bool doLoop){
	this->m_doLoop = doLoop;
	this->m_isActive = true;
	this->m_isPaused = false;
	if(Playing()) return true;
	if(!Stream(m_buffers[0])) return false;
	if(!Stream(m_buffers[1])) return false;

	alSourceQueueBuffers(m_source, N_PAIR, m_buffers);
	alSourcePlay(m_source);
	return true;
}

//Pause the music.
//pause - true to pause, false to unpause.
void SphMusic::Pause(bool pause){
	this->m_isPaused = pause;
	if(m_isPaused){
		alSourcePause(m_source);
	} else {
		alSourcePlay(m_source);
	}
}

//Update the music.
//return - true if the music is still active.
bool SphMusic::Update(){
	if(m_isPaused) return true;
	m_isActive = true;
	int processed;
	alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processed);

	while(processed > 0){
		ALuint buffer;
		alSourceUnqueueBuffers(m_source, 1, &buffer);
		m_isActive = Stream(buffer);
		alSourceQueueBuffers(m_source, 1, &buffer);
		processed--;
	}
	if(!m_isActive)Pause(true);
	return m_isActive;
}

//return - true if the music is active.
bool SphMusic::IsActive(){
	return m_isActive;
}

//Release all rem_sources associated with this music object.
void SphMusic::Free(){
	m_isPaused = true;
	m_isActive = false;
	alSourceStop(m_source);

	int queued;
	alGetSourcei(m_source, AL_BUFFERS_QUEUED, &queued);
	while(queued > 0){
		ALuint buffer;
		alSourceUnqueueBuffers(m_source, 1, &buffer);
		queued--;
	}

	alDeleteSources(1, &m_source);
	alDeleteBuffers(1, m_buffers);
	ov_clear(&m_oggStream);
}

//return - true if the m_source is playing.
bool SphMusic::Playing(){
	ALenum state;
	alGetSourcei(m_source, AL_SOURCE_STATE, &state);
	return (state == AL_PLAYING);
}

//Stream new data into the buffer.
//buffer - to fill with data.
//return - true if the music is still streaming.
bool SphMusic::Stream(ALuint buffer){
	char data[s_bufferSize];
	int size = 0;
	int section;

	while(size < s_bufferSize){
		int result = ov_read(&m_oggStream, data + size, s_bufferSize - size, 0, 2, 1, &section);
		if(result > 0){
			size += result;
		} else if(result < 0){
			return false;
		} else if(m_doLoop){
			ov_raw_seek(&m_oggStream, 0);
		} else {
			break;
		}
	}

	if(size == 0)return false;
	alBufferData(buffer, m_format, data, size, m_rate);
	return true;
}
