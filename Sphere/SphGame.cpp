#include "SphMath.h"
#include "SphSdlInput.h"
#include "SphGame.h"
#include "SphAvatar.h"
#include "SphDesertSkybox.h"
#include "SphDesertTerrain.h"
#include "SphFollowCamera.h"
#include "SphExplosionParticles.h"
#include "SphDesertWorld.h"
#include "SphHUD.h"
#include "SphMusic.h"
#include "SphOpenAL.h"
#include "SphOpenGL.h"

SphGame g_game;

//Create SphGame.
SphGame::SphGame(){
}

//Initialize SphGame.
//return - 0 on success.
int SphGame::Init(){
	m_exit = false;
	m_renderMode = 0;

	m_world = new SphDesertWorld();
	
	//Init avatar
	m_avatar = new SphAvatar();

	//Init GUI
	m_hud = new SphHUD(m_avatar);

	m_avatar->AddClosedType(SphAvatar::Normal);
	m_avatar->SetPosition(m_world->GetSpawnPosition());
	m_world->AddCollidable(m_avatar);

	//Create follow camera
	m_camera = new SphFollowCamera();
	((SphFollowCamera*)m_camera)->SetParams(10.0f, 10.0f, 1.0f);
	((SphFollowCamera*)m_camera)->m_object = m_avatar;

	m_skybox = new SphDesertSkybox();

	m_terrain = new SphDesertTerrain(m_avatar->GetPosition(), m_world);

	m_music = PlaySound("Audio/bg2.ogg", 0.75f, true);

	//this->SetWireframe(false);
	m_gameProgress = 0;

	return 0;
}

// returns true if audio loaded and played, false if no free channels or error
SphMusic* SphGame::PlaySound(cchar *filePath, float gain, bool loop)
{
	SphMusic *temp = g_openAL.GetMusic();
	if(temp != NULL){
		temp->OpenAndPlay(filePath, gain, loop);
		return temp;
	}
	else
	{
	}
	return NULL;

}

//Update SphGame.
void SphGame::Update(float seconds){

	static bool musicPaused = true;

	m_hud->Update(seconds);

	if(!(m_hud->MainMenuFocus() || m_hud->ConsoleFocus())){
		m_camera->Update(seconds);
		m_world->Update(seconds);
		m_skybox->update(m_avatar->GetPosition(), 180);
		m_terrain->update(m_avatar->GetPosition(), m_camera->GetPosition(), m_camera->GetForward(), m_world);
	}

	if(m_gameProgress == 0 && m_avatar->GetPosition()[O_Z] > 186.0f){
		m_music->Free();
		m_music = PlaySound("Audio/PreBattle.ogg", 0.75f, true);
		m_gameProgress++;
	}
	if(m_gameProgress == 1 && m_avatar->GetPosition()[O_Z] > 186.0f && m_avatar->GetPosition()[O_Y] > 110.0f){
		m_music->Free();
		m_music = PlaySound("Audio/Battle.ogg", 0.75f, true);
		m_gameProgress++;
	}
	if(m_gameProgress == 2 && m_avatar->GetPosition()[O_Y]>125.0f)
	{
		m_music->Free();
		m_music = PlaySound("Audio/bg1.ogg", 0.75f, true);
		m_gameProgress++;
	}
}

//Render SphGame.
void SphGame::Render(){
	if(m_camera) m_camera->ApplyCamera();
	
	m_skybox->render();
	
	//enable wireframe mode, if activated
	bool wireframesOn = (this->m_renderMode == 1);
	if(wireframesOn){
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		//glDisable(GL_LIGHTING); //I think it looks cooler with lighting, personally
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
	}
	else{
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glEnable(GL_LIGHTING);
		glEnable(GL_CULL_FACE);
	}

	m_world->Render();
	m_terrain->render(m_camera->GetPosition(),m_camera->GetForward());
	m_terrain->m_renderGrass();

	//clean up wireframe settings
	if(wireframesOn){
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glEnable(GL_LIGHTING);
		glEnable(GL_CULL_FACE);
	}

	m_skybox->render();//omg!
}

//Quit SphGame and free all resources.
void SphGame::Quit(){
	m_hud->Quit();
	m_avatar->Quit();
	m_world->Free();
}

void SphGame::Reset(){
	
	m_world->Free();
	m_world = new SphDesertWorld();
	m_avatar->Reset();
	m_avatar->SetPosition(m_world->GetSpawnPosition());
	m_world->AddCollidable(m_avatar);
	m_hud->Reset();

	m_music->Free();
	m_music = PlaySound("Audio/bg2.ogg", 0.75f, true);
	m_gameProgress = 0;
}

//Changes the rendering mode for all objects in the world
//	renderMode:	0 = default (filled polygons)
//				1 = wireframe mode
void SphGame::SetRenderMode(cint renderMode){
	this->m_renderMode = renderMode;
}
