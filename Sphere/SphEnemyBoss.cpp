#include "SphEnemyBoss.h"

SphEnemyBoss::SphEnemyBoss(){
}

/**
* Update
*
* Updates the boss according to the number of seconds that have passed
*
* args: seconds - The number of seconds since the last update
*/
void SphEnemyBoss::Update(float seconds){
	SphEnemy::Update(seconds);
}

/**
* CheckDoPreAttack
*
* Checks if the boss should perform its "pre-attack" action
*
* returns - true if the boss should do its pre-attack action
*			else false
*/
bool SphEnemyBoss::CheckDoPreAttack(){
	return 0;
}

/**
* CheckDoAttack
*
* Checks if the boss should perform its "attack" action
*
* returns - true if the boss should do its attack action
*			else false
*/
bool SphEnemyBoss::CheckDoAttack(){
	return 0;
}

/**
* PreAttackAction
*
* Performs the Pre-Attack state's action
*
* args: seconds - The seconds since the last update
*/
void SphEnemyBoss::PreAttackAction(float seconds){
	//fill it in derived classes, if needed
}