#include <string.h>
#include "SphMath.h"
#include "SphQuicksand.h"
#include "SphProp.h"
#include "SphGame.h"
#include "SphAvatar.h"
#include "SphSphericBound.h"

SphQuicksand::SphQuicksand(){
	m_quicksand1.Load("Meshes/Walls/", "QuickSand.drkMesh");
	SetMesh(&m_quicksand1);
	m_quicksandPlane = 6.5f;
	m_killPlane = m_quicksandPlane - 1.5f;
	Translate3(-100.0f, m_quicksandPlane, -100.0f);
}

//Update SphQuicksand.
//seconds - since the last call.
void SphQuicksand::Update(float seconds){
	float avatarY = (g_game.m_avatar->GetPosition())[O_Y] - ((SphSphericBound*)(g_game.m_avatar->m_bound))->m_radius;
	if(avatarY < m_quicksandPlane) {
		g_game.m_avatar->SetVelocity(g_zeroes);
		
		if(avatarY < m_killPlane) {
			g_game.m_avatar->TakeDamage(100);
		}
	}
}