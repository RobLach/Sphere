#include "SphMath.h"
#include "SphSdlInput.h"
#include "SphSdl.h"
#include "SphOpenGL.h"
#include "SphAvatarController.h"
#include "SphGame.h"
#include "SphAvatar.h"
#include "SphBound.h"
#include "SphRenderer.h"
#include "SphAction.h"
#include "SphHUD.h"

//Create SphAvatarController.
SphAvatarController::SphAvatarController(SphAvatar* avatar) : m_controller(3), m_legsController(4){
	this->m_avatar = avatar;
	this->SetupController();
	this->m_controller.SetModel(avatar);
	this->m_legsController.SetModel(avatar);
}

//Update SphAvatarController.
//seconds - since the last call.
void SphAvatarController::Update(float seconds){
	//compute forward and right forces
	float forward[N_XYZ], right[N_XYZ], up[N_XYZ];
	Set(up, 0.0f, 1.0f, 0.0f);
	Set(forward, g_game.m_camera->GetForward());
	forward[O_Y] = 0.0f;
	Normalize(forward);
	Multiply(forward, m_avatar->GetMoveForce());
	Cross(right, forward, up);
	Normalize(right);
	Multiply(right, m_avatar->GetMoveForce());
	float moveFactor = m_avatar->IsOnGround() ? 1.0f : 0.5f;//slow down movement in the air

	//set default transitions for animation controllers
	int transition = C_None;
	int legTransition = C_None;

	//process user's input
	if(g_sdlInput.IsKeyDown(SDLK_w)){
		Multiply(forward, moveFactor);
		m_avatar->AddForce(forward);
		transition = C_ForwardWalk;
	} else if(g_sdlInput.IsKeyDown(SDLK_s)){
		Multiply(forward, -moveFactor);
		m_avatar->AddForce(forward);	
		transition = C_ForwardWalk;
	}
	if(m_avatar->IsClosed() || m_avatar->HasAttachment(SphAvatar::Legs)){
		if(g_sdlInput.IsKeyDown(SDLK_d)){
			if(!IsClosed()){
				legTransition = C_StrafeRight;
			}
			Multiply(right, moveFactor);
			m_avatar->AddForce(right);
		} else if(g_sdlInput.IsKeyDown(SDLK_a)){
			if(!IsClosed()){
				legTransition = C_StrafeLeft;
			}
			Multiply(right, -moveFactor);
			m_avatar->AddForce(right);
		}
	}
	
	if(g_sdlInput.KeyUpEvent(SDLK_1)){
		m_avatar->SetClosedType(SphAvatar::Normal);
	} else if(g_sdlInput.KeyUpEvent(SDLK_2)){
		m_avatar->SetClosedType(SphAvatar::Rubber);
	} else if(g_sdlInput.KeyUpEvent(SDLK_3)){
		m_avatar->SetClosedType(SphAvatar::Speed);
	} else if(g_sdlInput.KeyUpEvent(SDLK_4)){
		m_avatar->SetClosedType(SphAvatar::Metal);
	}

	if(g_sdlInput.MouseButtonUpEvent(g_sdlInput.s_rightMouseButton)||g_sdlInput.KeyUpEvent(SDLK_g)){
		g_sdlInput.SwitchMouseControl();
	}
	if(g_sdlInput.IsKeyDown(SDLK_TAB)){
		if(IsClosed()){
			g_game.PlaySound("Audio/open.ogg", 1.0f, false);
		}
		m_avatar->SetRotation(g_unitQuat);//TODO: do actual animation
		transition = C_Open;
	}
	if(g_sdlInput.IsKeyDown(SDLK_SPACE)){
		if(this->m_avatar->HasAttachment(this->m_avatar->Legs))
		{
			legTransition = C_Jump;
			g_game.PlaySound("Audio/jump.ogg",1.0f, false);
		}
	}
	//if in open state, make sure the rotation is the same as camera's
	if(!IsClosed()){
		float forward[N_XYZ], rotation[N_QUAT];
		Set(forward, g_game.m_camera->GetForward());
		float angle = atan2(forward[O_X], forward[O_Z]);
		QuaternionY(rotation, angle);
		m_avatar->SetRotation(rotation);
	}

	//update animation controllers
	m_controller.SetActiveTransition(transition);
	m_legsController.SetActiveTransition(legTransition);
	m_controller.Update(seconds);
	m_legsController.Update(seconds);
}

//return - true if avatar is in closed form.
bool SphAvatarController::IsClosed(){
	return m_controller.GetActiveState() == 0;//closed
}

//Setup the animation controller states and transitions.
void SphAvatarController::SetupController(){
	int closed = m_controller.GetNewState();//0
	int opening = m_controller.GetNewState();
	int closing = m_controller.GetNewState();
	int stand = m_controller.GetNewState();
	int standToWalkR = m_controller.GetNewState();
	int walkR = m_controller.GetNewState();
	int walkL = m_controller.GetNewState();
	int walkRtoStand = m_controller.GetNewState();
	int walkLtoStand = m_controller.GetNewState();

	//									state					animation			C_None				C_ForwardWalk		C_Open
	m_controller.SetTransitions(closed,				A_Closed,			closed,				closed,				opening);
	m_controller.SetTransitions(opening,			A_Open,				stand,				standToWalkR,		closing);
	m_controller.SetTransitions(closing,			A_Close,				closed,				closed,				opening);
	m_controller.SetTransitions(stand,				A_Stand,				stand,				standToWalkR,		closing);
	m_controller.SetTransitions(standToWalkR,		A_StandToWalkR,	walkLtoStand,		walkR,				walkLtoStand);
	m_controller.SetTransitions(walkR,				A_WalkR,				walkRtoStand,		walkL,				walkRtoStand);
	m_controller.SetTransitions(walkL,				A_WalkL,				walkLtoStand,		walkR,				walkLtoStand);
	m_controller.SetTransitions(walkRtoStand,		A_WalkRToStand,	stand,				standToWalkR,		closing);
	m_controller.SetTransitions(walkLtoStand,		A_WalkLToStand,	stand,				standToWalkR,		closing);

	stand = m_legsController.GetNewState();
	int jump = m_legsController.GetNewState();
	int strafeLeft = m_legsController.GetNewState();
	int strafeRight = m_legsController.GetNewState();

	//										state				animation		C_None	C_Jump	C_StrafeLeft	C_StrafeRight
	m_legsController.SetTransitions(stand,			A_LegsStand,	stand,	jump,		strafeLeft,		strafeRight);
	m_legsController.SetTransitions(jump,			A_Jump,			stand,	jump,		strafeLeft,		strafeRight);
	m_legsController.SetTransitions(strafeLeft,	A_StrafeLeft,	stand,	jump,		strafeLeft,		strafeRight);
	m_legsController.SetTransitions(strafeRight,	A_StrafeRight,	stand,	jump,		strafeLeft,		strafeRight);
}