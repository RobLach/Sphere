#pragma once
#include "al.h"
#include <vorbis/vorbisfile.h>
#include "SphInclude.h"

//SphMusic is borrowed with some modifications from DevMaster (http://www.devmaster.net) OpenAL lesson #8.
//SphMusic can load and play an OGG music/sound file.
//SphMusic streams the file, so it's optimal for playing big files.
class SphMusic {

private:
	static cint	s_bufferSize = 32 * N_KILOBYTE;

	OggVorbis_File m_oggStream;
	ALuint m_buffers[N_PAIR];
	ALuint m_source;
	ALenum m_format;
	ALsizei m_rate;
	bool m_isActive;
	bool m_isPaused;
	bool m_doLoop;

	bool Playing();
	bool Stream(ALuint buffer);

public:
	SphMusic();
	int Open(cchar* filePath);
	int OpenAndPlay(cchar* filePath);
	int OpenAndPlay(cchar* filePath, float gain, bool loopForever);
	bool StartPlaying(bool doLoop);
	void Pause(bool pause);
	bool Update();
	bool IsActive();
	void SetPosition(cfloat* position);
	void SetVelocity(cfloat* velocity);
	void SetDirection(cfloat* direction);
	void SetRolloff(cfloat rolloff);
	void SetVolume(cfloat gain);
	void Free();
};
