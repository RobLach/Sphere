#include "SphMath.h"
#include "SphEnemy.h"
#include "SphGame.h"
#include "SphCamera.h"
#include "SphAvatar.h"
#include "SphWorld.h"
#include "SphSphericBound.h"
#include "SphCollisionDetection.h"
#include "SphProjectile.h"
#include "SphBasicAgent.h"


//Create SphEnemy.
SphEnemy::SphEnemy(){
	this->m_bound = new SphSphericBound(this);
	this->m_health = 100;
	this->m_flying = false;
	this->m_damage = 1;
	this->m_newAttack = false;
}

//Renderer SphEnemy.
void SphEnemy::Render(){
	SphModel::Render();
}

// Returns the amount of damage the enemy deals.
int SphEnemy::GetDamage() {
	return m_damage;
}

SphEnemy* l_collisionEnemy = NULL;

//return - true if first collision is closer to avatar.
bool SortEnemyCollisions(SphCollisionInfo first, SphCollisionInfo second){
	cfloat* enemyPos = l_collisionEnemy->GetPosition();
	return DistanceSq(enemyPos, first.m_collisionPoint) < DistanceSq(enemyPos, second.m_collisionPoint);
}

//Process collisions with avatar.
void SphEnemy::ProcessCollisions(){
	//collide with normal objects
	SphCollisionsList collisionsList;
	g_game.m_world->GetCollisions(&collisionsList, m_bound);
	l_collisionEnemy = this;
	collisionsList.sort(SortEnemyCollisions);
	for(SphCollisionsList::iterator iter = collisionsList.begin(); iter != collisionsList.end(); iter++){
		ProcessCollision(&*iter);
	}
}

//info - collision info to process.
void SphEnemy::ProcessCollision(SphCollisionInfo* info){
	//translate the object far enough from the collision point
	float fix[N_XYZ], diff[N_XYZ], magnitudeDiff;
	float radius = ((SphSphericBound*)m_bound)->m_radius;
	Subtract(diff, GetPosition(), info->m_collisionPoint);
	magnitudeDiff = Magnitude(diff);
	if(magnitudeDiff > radius) return;

	Multiply(diff, 1.0f / magnitudeDiff);
	Set(fix, diff);
	Multiply(fix, radius - magnitudeDiff);
	Translate(fix);

	//adjust velocity based on collision normal
	float proj[N_XYZ], velocity[N_XYZ];
	Set(velocity, GetVelocity());
	VectorProject(proj, velocity, diff);
	SubtractFrom(velocity, proj);
	Multiply(proj, -0.2);//default bounce factor
	AddTo(velocity, proj);
	SetVelocity(velocity);

	// Land-based enemies do damage on collision
	if(((info->m_object)->GetObjType() == SphObject::AVATAR_OBJ) &&
		(! ((SphEnemy*)this)->m_flying) && (this->m_newAttack))
	{
		(info->m_object)->TakeDamage(this->GetDamage());
		this->m_newAttack = false;
	}
}

//Update SphEnemy.
//seconds - since the last call.
void SphEnemy::Update(float seconds){
	//=======================================TEMPORARILY COMMENTED OUT
	m_agent->ExecuteState(seconds);

	ApplyPhysics(seconds, 0.93f, 0.0f, !m_flying);//0.93f is just temporary, until we can figure out what surface the avatar is in contact with
	ProcessCollisions();
	SphModel::Update(seconds);
}

//Take damage function
void SphEnemy::TakeDamage(int damage){
	this->m_health -= damage;
}

//Insert Enemy AI here
void SphEnemy::Attack(SphObject* target){
}

//return - this object's type.
SphObject::SphObjectType SphEnemy::GetObjType(){
	return ENEMY_OBJ;
}



/***********************************
*
*	IBaseAgent INTERFACE
*
***********************************/
//return - if the enemy is "safe"
bool SphEnemy::CheckIsSafe() {
	//TODO: change this
	return (this->GetHealth() > 10);
}

//return - if the enemy should "run away"
bool SphEnemy::CheckRunAway() {
	//TODO: change this
	return 0;
}

//return - if the enemy has reached its destination
bool SphEnemy::CheckDestinationReached() {
	//TODO: change this
	return 0;
}

//Executes what to do when in a "hostile" state
void SphEnemy::HostileAction(float seconds) {
	//TODO: change this
	//this->Attack(......);
}

//Executes what to do when in a "avoid" state
void SphEnemy::AvoidAction(float seconds) {
	//TODO: change this
}

//Executes what to do when in an "idle" state
void SphEnemy::IdleAction(float seconds) {
	//TODO: change this to some "patrolling" pattern
	float force[N_XYZ];
	Set(force, 0.f, 0.f, 10.f);
	this->AddForce(force);
}

//Executes what to do when in a "move" state
void SphEnemy::MoveAction(float seconds) {
}