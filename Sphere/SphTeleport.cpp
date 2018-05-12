#include "SphMath.h"
#include "SphOpenGL.h"
#include "SphTeleport.h"
#include "SphGame.h"
#include "SphAvatar.h"

SphTeleport::SphTeleport(float x, float y, float z){
	Set(m_dest, x, y, z);
	m_texture = g_openGL.LoadTexture("Textures/Portal.tga");
}

void SphTeleport::Update(float seconds){
	if(Distance(g_game.m_avatar->GetPosition(), GetPosition()) < 9.0f){
		g_game.PlaySound("Audio/teleport.ogg", 1.0f, false);
		g_game.m_avatar->SetPosition(m_dest);
		g_game.m_avatar->Teleporting();
	}
}