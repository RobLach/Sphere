#include "SphMath.h"
#include "SphCubeEnemy.h"
#include "SphMesh.h"
#include "SphGame.h"
#include "SphCamera.h"
#include "SphAvatar.h"
#include "SphWorld.h"
#include "SphTerrain.h"
#include "SphDesertTerrain.h"
#include "SphSphericBound.h"
#include "SphEnemyCubeAgent.h"
#include "SphEnemyCubeStates.h"

//HP that determines when the cube should start fleeing
cint HP_fleeThreshold = 35;

//Create SphCubeEnemy.
SphCubeEnemy::SphCubeEnemy(){
	this->Load("Meshes/Enemies/", "EvilCube.drk");

	this->m_damagedByLaser = true;
	this->m_damagedBySpray = true;
	this->m_timeSinceLastAction = 0.f;
	this->m_secondsBetweenActions = 1.5f;
	this->m_damage = 20;
	//Add AI to enemy
	SphEnemyCubeIdleState* startState = new SphEnemyCubeIdleState();
	m_agent = new SphEnemyCubeAgent(this, startState);
}

void SphCubeEnemy::Update(float seconds){
	SphEnemy::Update(seconds);
	float enemyPos[N_XYZ]; Set(enemyPos, this->GetPosition());
	float enemyRadius = ((SphSphericBound*)(this->m_bound))->m_radius;
	float enemy_y = enemyPos[O_Y] - enemyRadius;
	float terrainHeight = g_game.m_terrain->getHeightc(enemyPos[O_X], enemyPos[O_Z]);
	if(terrainHeight > enemy_y)
	{
		this->SetPosition(enemyPos[O_X], terrainHeight + enemyRadius, enemyPos[O_Z]);
		this->SetVelocity(g_zeroes);
	}
	float rot[N_QUAT];
	float avatarPos[N_XYZ]; Set(avatarPos, g_game.m_avatar->GetPosition());
	float avatarDirection[N_XYZ];
	if(Distance(avatarPos,enemyPos) < 650.0f){
		Set(avatarDirection, enemyPos[O_X] - avatarPos[O_X], enemyPos[O_Y] - avatarPos[O_Y], enemyPos[O_Z] - avatarPos[O_Z]);
		Normalize(avatarDirection);
		QuaternionY(rot , atan2(avatarDirection[O_X],avatarDirection[O_Z])+PI);
		this->SetRotation(rot);
	}
	
}


/***********************************
*
*	IBaseAgent INTERFACE
*
***********************************/
//return - if the enemy is "safe"
bool SphCubeEnemy::CheckIsSafe() {

	if(this->m_secondsBetweenActions < this->m_timeSinceLastAction){
		float avatarPos[N_XYZ]; Set(avatarPos, g_game.m_avatar->GetPosition());
		float enemyPos[N_XYZ]; Set(enemyPos, this->GetPosition());

		if(Distance(avatarPos,enemyPos) < 550.0f){
			return 0;
		}
	}
	
	return 1;
}

//return - if the enemy should "run away"
bool SphCubeEnemy::CheckRunAway() {
	
	if((this->GetHealth() < HP_fleeThreshold)){
		float avatarPos[N_XYZ]; Set(avatarPos, g_game.m_avatar->GetPosition());
		float enemyPos[N_XYZ]; Set(enemyPos, this->GetPosition());

		if((Distance(avatarPos, enemyPos) < 50.0f)){
			return 1;
		}
	}
	
	return 0;
}

//return - if the enemy has reached its destination
bool SphCubeEnemy::CheckDestinationReached() {
	//TODO: change this
	return 0;
}

//Executes what to do when in a "hostile" state
void SphCubeEnemy::HostileAction(float seconds) {

	this->m_newAttack = true;

	float force[N_XYZ];
	float avatarPos[N_XYZ];
	float enemyPos[N_XYZ];
	float directionVec[N_XY];
	Set(avatarPos, g_game.m_avatar->GetPosition());
	Set(enemyPos, this->GetPosition());

	float xVec = (enemyPos[O_X]+avatarPos[O_X])/2.0f - enemyPos[O_X];
	float yVec = (enemyPos[O_Z]+avatarPos[O_Z])/2.0f - enemyPos[O_Z];
	Set(directionVec, xVec, yVec);

	Normalize2(directionVec);

	if((Distance(avatarPos, enemyPos) < 200.0f)){
		Set(force, directionVec[O_X] * 1200.0f, 80.0f , directionVec[O_Y] * 1200.0f); // SMASH!
		this->m_timeSinceLastAction = 0.0f;
	}
	else if((avatarPos[O_Y]-5.0f)>enemyPos[O_Y]){
		Set(force, directionVec[O_X] * 350.0f, 400.0f , directionVec[O_Y] * 350.0f); // High-Hop
		this->m_timeSinceLastAction = 0.5f;
	}
	else
	{
		Set(force, directionVec[O_X] * 350.0f, 200.0f , directionVec[O_Y] * 350.0f); // Hop
		this->m_timeSinceLastAction = 0.5f;
	}
	this->AddForce(force);
}

//Executes what to do when in a "avoid" state
void SphCubeEnemy::AvoidAction(float seconds) {
	if(Magnitude(this->GetVelocity()) <0.1f)
	{
	float force[N_XYZ];
	float avatarPos[N_XYZ];
	float enemyPos[N_XYZ];
	float directionVec[N_XY];
	if(g_game.m_avatar->GetPosition() != NULL)
	
	Set(avatarPos, g_game.m_avatar->GetPosition());
	Set(enemyPos, this->GetPosition());
	float xVec = (enemyPos[O_X]+avatarPos[O_X])/2.0f - avatarPos[O_X];
	float yVec = (enemyPos[O_Z]+avatarPos[O_Z])/2.0f - avatarPos[O_Z];
	Set(directionVec, xVec, yVec);

	Normalize2(directionVec);
	Set(force, directionVec[O_X], 
		5.0f , directionVec[O_Y]);
	this->AddForce(force);
	}
}

//Executes what to do when in an "idle" state
void SphCubeEnemy::IdleAction(float seconds) {
	//update time this object has been idle
	this->m_timeSinceLastAction += seconds;
}

//Executes what to do when in a "move" state
void SphCubeEnemy::MoveAction(float seconds) {

}

