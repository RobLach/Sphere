#include "SphMath.h"
#include "SphAnubisBoss.h"
#include "SphMesh.h"
#include "SphGame.h"
#include "SphCamera.h"
#include "SphAvatar.h"
#include "SphWorld.h"
#include "SphEnemyAnubisAgent.h"
#include "SphEnemyAnubisStates.h"
#include "SphTightBound.h"
#include "SphDesertTerrain.h"

//Create SphAnubisBoss.
SphAnubisBoss::SphAnubisBoss(){
	this->Load("Meshes/Enemies/", "Anubis.drk");
	this->m_bound = new SphTightBound(this);
	((SphTightBound*)m_bound)->Load("Meshes/Enemies/Anubis.bound.drkMesh");

	this->m_health = 1;

	this->m_damagedByLaser = true;
	this->m_damagedBySpray = true;

	//pre-attack state delay variables
	this->m_timeInPreAttackState = 0.0f;
	this->m_secondsOfPreAttackDelay = 5.0f;
	//time between when an attack ends and a new pre-attack->attack starts
	this->m_timeSinceLastAttack = 0.0f;
	this->m_secondsBetweenAttacks = 5.0f;

	//Add AI to enemy
	SphEnemyAnubisIdleState* startState = new SphEnemyAnubisIdleState();
	m_agent = new SphEnemyAnubisAgent(this, startState);
}

//Update anubis
void SphAnubisBoss::Update(float seconds){
	//SphEnemyBoss::Update(seconds);
	m_agent->ExecuteState(seconds);
	SphModel::Update(seconds);
	float enemyPos[N_XYZ]; Set(enemyPos, this->GetPosition());
	
	float rot[N_QUAT];
	float avatarPos[N_XYZ]; Set(avatarPos, g_game.m_avatar->GetPosition());
	float avatarDirection[N_XYZ];
	
	Set(avatarDirection, enemyPos[O_X] - avatarPos[O_X], enemyPos[O_Y] - avatarPos[O_Y], enemyPos[O_Z] - avatarPos[O_Z]);
	Normalize(avatarDirection);
	QuaternionY(rot , atan2(avatarDirection[O_X],avatarDirection[O_Z])+PI);
	this->SetRotation(rot);

	
}


/***********************************
*
*	IBaseEnemyAgent INTERFACE
*
***********************************/
//return - if the enemy is "safe"
bool SphAnubisBoss::CheckIsSafe() {

	//check if the enemy is within range
	float avatarPos[N_XYZ]; Set(avatarPos, g_game.m_avatar->GetPosition());
	float enemyPos[N_XYZ]; Set(enemyPos, this->GetPosition());

	//TODO: this is attack height, adjust where necessary
	if(abs(enemyPos[O_Y] - avatarPos[O_Y]) <= 23.0f){
		return 0;
	}
	
	return 1;
}

//return - if the enemy has reached its destination
bool SphAnubisBoss::CheckDestinationReached() {
	//TODO: change this if we're going to use it
	return 0;
}

//Executes what to do when in a "hostile" state
void SphAnubisBoss::HostileAction(float seconds) {
	//TODO: make it rain...
	//printf("MAKIN' IT RAIN, DAWG...\n");
	this->PlayAction(0);//temporary play attack animation

	g_game.m_avatar->SetOnFire();
	//reset time in pre-attack state (causes transition back to idle state)
	this->m_timeInPreAttackState = 0;
	//reset time since last attack 
	this->m_timeSinceLastAttack = 0;
}

//Executes what to do when in an "idle" state
void SphAnubisBoss::IdleAction(float seconds) {
	//update time between this object's attacks
	this->m_timeSinceLastAttack += seconds;
}

//Executes what to do when in a "move" state
void SphAnubisBoss::MoveAction(float seconds) {
	//probably don't need this
}

/***********************************
*
*	IBaseEnemyBossAgent INTERFACE
*
***********************************/
//return - true if anubis should do his pre-attack action
bool SphAnubisBoss::CheckDoPreAttack(){
	//do attack after certain # of seconds in idle state,
	//	-modify according to health???

	if(this->m_secondsBetweenAttacks < this->m_timeSinceLastAttack){
		//reset the time in pre-attack state, because that's where we'll be transitioning
		this->m_timeInPreAttackState = 0.0f;
		return 1;
	}

	return 0;
}

//return - true if anubis should do his attack action
bool SphAnubisBoss::CheckDoAttack(){
	//do attack after certain # of seconds in pre-attack state
	if(this->m_secondsOfPreAttackDelay < this->m_timeInPreAttackState){
		return 1;
	}
	return 0;
}

//Executes the action of the "pre-attack" state
void SphAnubisBoss::PreAttackAction(float seconds){
	
	
	if(this->m_timeInPreAttackState == 0.0f)
	{
		//TODO: start raise arms animation on first execution of this state
		printf("PRE-ATTACK...\n");
		g_game.PlaySound("Audio/growl.ogg", 0.8f, false);
	}

	//keep track of how long we've been delaying actual attack
	this->m_timeInPreAttackState += seconds;
}