#include "TestHarness.h"
#include "SphOpenAL.h"
#include "al.h"
#include <vorbis/vorbisfile.h>
#include "SphInclude.h"
#include "SDL.h"
#include <stdio.h>

TEST( Audio, complete)
{
	printf("---Audio Tests Done\n");
}

// Tests for audio freeing
TEST( Audio, musicFree)
{
	printf("-Running Audio Music Freeing Test - ");
	g_openAL.Init();
	SphMusic* test = g_openAL.GetMusic();
	test->Open("Audio/summer.ogg");
	test->StartPlaying(0);
	SDL_Delay(500);
	test->Free();
	bool test2 = test->IsActive();
	CHECK(test2 == false);
	printf(" DONE \n");
	g_openAL.Quit();
}

// Tests for audio update
TEST( Audio, musicUpdate)
{
	printf("-Running Audio Music Update Test - ");
	g_openAL.Init();
	SphMusic* test = g_openAL.GetMusic();
	test->Open("Audio/summer.ogg");
	test->StartPlaying(0);
	SDL_Delay(500);
	bool test2 = test->Update();
	CHECK(test2 == 1);
	printf(" DONE \n");
	test->Free();
	g_openAL.Quit();
}

// Tests if music is active
TEST( Audio, musicActive)
{
	printf("-Running Audio Music isActive Test - ");
	g_openAL.Init();
	SphMusic* test = g_openAL.GetMusic();
	test->Open("Audio/summer.ogg");
	test->StartPlaying(0);
	SDL_Delay(500);
	bool test2 = test->IsActive();
	CHECK(test2 == 1);
	printf(" DONE \n");
	test->Free();
	g_openAL.Quit();
}

// Tests for Music Playback
TEST( Audio, musicPlay13)
{
	printf("-Running Audio Music Playing Test - Audio/explosion2.ogg");
	g_openAL.Init();
	SphMusic* test = g_openAL.GetMusic();
	test->Open("Audio/explosion2.ogg");
	bool test2 = test->StartPlaying(0);
	SDL_Delay(500);
	CHECK(test2 == 1);
	printf(" DONE \n");
	test->Free();
	g_openAL.Quit();
}

// Tests for Music Playback
TEST( Audio, musicPlay12)
{
	printf("-Running Audio Music Playing Test - Audio/teleport.ogg");
	g_openAL.Init();
	SphMusic* test = g_openAL.GetMusic();
	test->Open("Audio/teleport.ogg");
	bool test2 = test->StartPlaying(0);
	SDL_Delay(500);
	CHECK(test2 == 1);
	printf(" DONE \n");
	test->Free();
	g_openAL.Quit();
}

// Tests for Music Playback
TEST( Audio, musicPlay11)
{
	printf("-Running Audio Music Playing Test - Audio/spray.ogg");
	g_openAL.Init();
	SphMusic* test = g_openAL.GetMusic();
	test->Open("Audio/spray.ogg");
	bool test2 = test->StartPlaying(0);
	SDL_Delay(500);
	CHECK(test2 == 1);
	printf(" DONE \n");
	test->Free();
	g_openAL.Quit();
}
// Tests for Music Playback
TEST( Audio, musicPlay10)
{
	printf("-Running Audio Music Playing Test - Audio/Battle.ogg");
	g_openAL.Init();
	SphMusic* test = g_openAL.GetMusic();
	test->Open("Audio/Battle.ogg");
	bool test2 = test->StartPlaying(0);
	SDL_Delay(500);
	CHECK(test2 == 1);
	printf(" DONE \n");
	test->Free();
	g_openAL.Quit();
}
// Tests for Music Playback
TEST( Audio, musicPlay9)
{
	printf("-Running Audio Music Playing Test - Audio/PreBattle.ogg");
	g_openAL.Init();
	SphMusic* test = g_openAL.GetMusic();
	test->Open("Audio/PreBattle.ogg");
	bool test2 = test->StartPlaying(0);
	SDL_Delay(500);
	CHECK(test2 == 1);
	printf(" DONE \n");
	test->Free();
	g_openAL.Quit();
}
// Tests for Music Playback
TEST( Audio, musicPlay8)
{
	printf("-Running Audio Music Playing Test - Audio/bg2.ogg");
	g_openAL.Init();
	SphMusic* test = g_openAL.GetMusic();
	test->Open("Audio/bg2.ogg");
	bool test2 = test->StartPlaying(0);
	SDL_Delay(500);
	CHECK(test2 == 1);
	printf(" DONE \n");
	test->Free();
	g_openAL.Quit();
}
// Tests for Music Playback
TEST( Audio, musicPlay7)
{
	printf("-Running Audio Music Playing Test - Audio/pickUp.ogg");
	g_openAL.Init();
	SphMusic* test = g_openAL.GetMusic();
	test->Open("Audio/pickUp.ogg");
	bool test2 = test->StartPlaying(0);
	SDL_Delay(500);
	CHECK(test2 == 1);
	printf(" DONE \n");
	test->Free();
	g_openAL.Quit();
}
// Tests for Music Playback
TEST( Audio, musicPlay6)
{
	printf("-Running Audio Music Playing Test - Audio/jump.ogg");
	g_openAL.Init();
	SphMusic* test = g_openAL.GetMusic();
	test->Open("Audio/jump.ogg");
	bool test2 = test->StartPlaying(0);
	SDL_Delay(500);
	CHECK(test2 == 1);
	printf(" DONE \n");
	test->Free();
	g_openAL.Quit();
}
// Tests for Music Playback
TEST( Audio, musicPlay5)
{
	printf("-Running Audio Music Playing Test - Audio/fire.ogg");
	g_openAL.Init();
	SphMusic* test = g_openAL.GetMusic();
	test->Open("Audio/fire.ogg");
	bool test2 = test->StartPlaying(0);
	SDL_Delay(500);
	CHECK(test2 == 1);
	printf(" DONE \n");
	test->Free();
	g_openAL.Quit();
}
// Tests for Music Playback
TEST( Audio, musicPlay4)
{
	printf("-Running Audio Music Playing Test - Audio/rubberBounce.ogg");
	g_openAL.Init();
	SphMusic* test = g_openAL.GetMusic();
	test->Open("Audio/rubberBounce.ogg");
	bool test2 = test->StartPlaying(0);
	SDL_Delay(500);
	CHECK(test2 == 1);
	printf(" DONE \n");
	test->Free();
	g_openAL.Quit();
}

// Tests for Music Playback
TEST( Audio, musicPlay3)
{
	printf("-Running Audio Music Playing Test - Audio/growl.ogg");
	g_openAL.Init();
	SphMusic* test = g_openAL.GetMusic();
	test->Open("Audio/growl.ogg");
	bool test2 = test->StartPlaying(0);
	SDL_Delay(500);
	CHECK(test2 == 1);
	printf(" DONE \n");
	test->Free();
	g_openAL.Quit();
}

// Tests for Music Playback
TEST( Audio, musicPlay2)
{
	printf("-Running Audio Music Playing Test - Audio/laser.ogg");
	g_openAL.Init();
	SphMusic* test = g_openAL.GetMusic();
	test->Open("Audio/laser.ogg");
	bool test2 = test->StartPlaying(0);
	SDL_Delay(500);
	CHECK(test2 == 1);
	printf(" DONE \n");
	test->Free();
	g_openAL.Quit();
}

// Tests for Music Playback
TEST( Audio, musicPlay1)
{
	printf("-Running Audio Music Playing Test - ");
	g_openAL.Init();
	SphMusic* test = g_openAL.GetMusic();
	test->Open("Audio/open.ogg");
	bool test2 = test->StartPlaying(0);
	SDL_Delay(500);
	CHECK(test2 == 1);
	printf(" DONE \n");
	test->Free();
	g_openAL.Quit();
}

// Tests for opening music
TEST( Audio, musicOpen)
{
	printf("-Running Audio Music Loading Test - ");
	g_openAL.Init();
	SphMusic* test = g_openAL.GetMusic();
	CHECK(test->Open("Audio/open.ogg") == 0);
	printf(" DONE \n");
	test->Free();
	g_openAL.Quit();
}

// Tests for valid Music Object
TEST( Audio, getMusicObject )
{
  printf("-Running Audio Music Object Test - ");
  g_openAL.Init();
  CHECK(g_openAL.GetMusic() != NULL);
  printf(" DONE \n");
  g_openAL.Quit();

}

// Tests for initialization of audio engine
TEST( Audio, initialization )
{
  printf("-Running Audio Initialization Test - ");
  CHECK( g_openAL.Init() == 0);
  g_openAL.Quit();
  printf(" DONE \n");
}




