#pragma once
#include "SphInclude.h"

class SphCamera;
class SphAvatar;
class SphHUD;
class SphWorld;
class SphDesertSkybox;
class SphDesertTerrain;
class SphMusic;

//SphGame consists of all objects and rules necessary for a game.
class SphGame{

private:
	int m_renderMode;

public:
	SphAvatar* m_avatar;
	SphCamera* m_camera;
	SphWorld* m_world;
	SphHUD* m_hud;
	SphDesertTerrain* m_terrain;
	SphDesertSkybox* m_skybox;
	SphMusic* m_music;
	int m_gameProgress;
	
	//temp code
	bool m_exit;

 	SphGame();
	int Init();
	SphMusic* PlaySound(cchar* filePath, float gain, bool loop);
	virtual void SetRenderMode(cint renderMode);
	void Update(float seconds);
	void Render();
	void Quit();
	void Reset();
	
};

extern SphGame g_game;
