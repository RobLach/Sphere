#pragma once
#include "SphInclude.h"
#include "SphFollowCamera.h"
#include "SphAnimationController.h"

class SphAvatar;

//SphAvatarController controls avatar's animation.
class SphAvatarController {

private:
	enum Action { A_Close, A_Closed, A_Default, A_Jump, A_LegsStand, A_Open, A_Stand, A_StandToWalkR, A_StrafeLeft, A_StrafeRight, A_WalkL, A_WalkLToStand, A_WalkR, A_WalkRToStand };
	enum ControlAlphabet { C_None = 0, C_ForwardWalk, C_Open };
	enum LegsAlphabet { C_Jump = 1, C_StrafeLeft, C_StrafeRight };

	SphAvatar* m_avatar;
	SphAnimationController m_controller;//controls the animation state of the avatar
	SphAnimationController m_legsController;//controls the animation state of avatar's legs

	void SetupController();

public:
	SphAvatarController(SphAvatar* avatar);
	void Update(float seconds);
	bool IsClosed();
};
