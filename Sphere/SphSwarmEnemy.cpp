#include "SphMath.h"
#include "SphOpenGL.h"
#include "SphSdlInput.h"
#include "SphSwarmEnemy.h"
#include "SphMesh.h"
#include "SphSphericBound.h"
#include "SphEnemySwarmAgent.h"
#include "SphEnemySwarmStates.h"
#include "SphGame.h"
#include "SphAvatar.h"
#include "SphWorld.h"
#include "SphTerrain.h"
#include "SphDesertTerrain.h"

SphMesh* SphSwarmEnemy::s_beeMesh = NULL;

cint HP_fleeThreshold = 99;

//Create SphSwarmEnemy.
SphSwarmEnemy::SphSwarmEnemy(){
	if(s_beeMesh == NULL){
		s_beeMesh = new SphMesh("Meshes/Enemies/", "Bee.drkMesh");
	}
	m_time = 0.0f;
	m_flying = true;
	((SphSphericBound*)m_bound)->m_radius = 3.0f;
	//DELETE(m_bound);
	this->m_damagedByLaser = false;
	this->m_damagedBySpray = true;
	this->m_timeSinceLastAction = 0.f;
	this->m_secondsBetweenActions = 0.5f;

	//Add AI to enemy
	SphEnemySwarmIdleState* startState = new SphEnemySwarmIdleState();
	m_agent = new SphEnemySwarmAgent(this, startState);
}

//Renderer SphSwarmEnemy.
void SphSwarmEnemy::Render(){
	glPushMatrix();
	ApplyMatrix();
	for(int bee = 0; bee < s_beeCount; bee++){
		glPushMatrix();
		glTranslatef(m_beesPos[bee * N_XYZ + O_X], m_beesPos[bee * N_XYZ + O_Y], m_beesPos[bee * N_XYZ + O_Z]);
		s_beeMesh->Render();
		glPopMatrix();
	}
	glPopMatrix();
}

//Update SphSwarmEnemy.
//seconds - since the last call.
void SphSwarmEnemy::Update(float seconds){
	m_time += seconds;
	srand(0);
	for(int bee = 0; bee < s_beeCount; bee++){
		float o1 = Randf(), o2 = Randf(), o3 = Randf(), o4 = Randf(), o5 = Randf(), o6 = Randf();
		Set(m_beesPos + bee * N_XYZ, Randf2() + sin(2.1f * (m_time + PI2 * o1)) * sin(1.3f * (m_time + PI2 * o2)),
			                          Randf2() + sin(2.3f * (m_time + PI2 * o3)) * sin(1.7f * (m_time + PI2 * o4)),
											  Randf2() + sin(2.7f * (m_time + PI2 * o5)) * sin(1.5f * (m_time + PI2 * o6)));
		Multiply(m_beesPos + bee * N_XYZ, 1.4f);
	}
	SphEnemy::Update(seconds);

	// Make sure the entire swarm doesn't go through the ground
	float enemyPos[N_XYZ]; Set(enemyPos, this->GetPosition());
	float enemyRadius = 3;//((SphSphericBound*)(this->m_bound))->m_radius;
	float enemy_y = enemyPos[O_Y] - enemyRadius;
	float terrainHeight = g_game.m_terrain->getHeightc(enemyPos[O_X], enemyPos[O_Z]);
	if(terrainHeight > enemy_y)
	{
		this->SetPosition(enemyPos[O_X], terrainHeight + enemyRadius, enemyPos[O_Z]);
		this->SetVelocity(g_zeroes);
	}
}

/***********************************
*
*
*
***********************************/
//return - if the enemy is "safe"
bool SphSwarmEnemy::CheckIsSafe() {

	if(this->m_secondsBetweenActions < this->m_timeSinceLastAction){
		float avatarPos[N_XYZ], enemyPos[N_XYZ];
		Set(avatarPos, g_game.m_avatar->GetPosition());
		Set(enemyPos, this->GetPosition());

		if(Distance(avatarPos,enemyPos) < 900.0f){
			return 0;
		}
	}
	
	return 1;
}

//return	IBaseAgent INTERFACE - if the enemy should "run away"
bool SphSwarmEnemy::CheckRunAway() {
	// Swarm will never run away, always return 0
	return 0;
}

//return - if the enemy has reached its destination
bool SphSwarmEnemy::CheckDestinationReached() {

	float delta[N_XYZ];
	Subtract(delta, (g_game.m_avatar)->GetPosition(), this->GetPosition());
	Normalize(delta);

	// If true, we reached the avatar
	if( (float) Magnitude(delta) < 5.0) {
		return 1;
	}

	return 0;
}

//Executes what to do when in a "hostile" state
void SphSwarmEnemy::HostileAction(float seconds) {

	this->m_timeSinceLastAction = 0.0f;

	float force[N_XYZ], target[N_XYZ], avatarPos[N_XYZ], enemyPos[N_XYZ], destTarget[N_XYZ], rotq[N_QUAT], myRot = 0.0, myAxis[N_XYZ];
	//SphAvatar* avatar = g_game.m_avatar;
	Set(avatarPos, g_game.m_avatar->GetPosition());
	Set(enemyPos, this->GetPosition());
	Set(target, 0.0f, 0.0f, 10000.0f);//default forward direction
	Set(destTarget, (enemyPos[O_X]+avatarPos[O_X])/2.0f - enemyPos[O_X] , 
			(enemyPos[O_Y]+avatarPos[O_Y])/2.0f - enemyPos[O_Y],
			(enemyPos[O_Z]+avatarPos[O_Z])/2.0f - enemyPos[O_Z]);

	Normalize(target);
	Normalize(destTarget);

	// Move the swarm towards the avatar
	Set(force, destTarget[O_X] * 350.0f, destTarget[O_Y] * 550.0f , destTarget[O_Z] * 350.0f);
	this->AddForce(force);

	// Rotate the swarm to face the avatar
	myRot = cos(Dot2(target,destTarget));
	Cross(myAxis,target,destTarget);
	Normalize(myAxis);
	AxisAngleToQuat(rotq, myAxis, myRot);
	this->SetRotation(rotq);

	// Avatar takes damage if it is within range
	if((Distance(avatarPos, enemyPos) < 50.0f)){
		(g_game.m_avatar)->TakeDamage(this->GetDamage());

	}
	
	if(g_game.m_avatar->HasAttachment(SphAvatar::SprayGun)){
		if(g_sdlInput.MouseButtonDownEvent(g_sdlInput.s_leftMouseButton)){
			if((Distance(avatarPos, enemyPos) < 100.0f)){
				TakeDamage(25);
			}
		}
	}


}

//Executes what to do when in a "avoid" state
void SphSwarmEnemy::AvoidAction(float seconds) {
	// Swarm doesn't try to avoid its target
}

//Executes what to do when in an "idle" state
void SphSwarmEnemy::IdleAction(float seconds) {
	//Swarm just stays in its spot

	//update time this object has been idle
	this->m_timeSinceLastAction += seconds;
}

//Executes what to do when in a "move" state
void SphSwarmEnemy::MoveAction(float seconds) {
	// Swarm does no additional actions while moving
}