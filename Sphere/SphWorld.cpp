#include "SphMath.h"
#include "SphWorld.h"
#include "SphObject.h"
#include "SphItem.h"
#include "SphProjectile.h"
#include "SphGame.h"
#include "SphOpenGL.h"

//Create SphWorld.
SphWorld::SphWorld(){
	Set(m_startPos, g_zeroes);
	this->m_explosion = new SphExplosionParticles("./Textures/Particles/", "debris.tga", g_zeroes);
}

//Render SphWorld.
void SphWorld::Render(){
	for(SphObjectList::iterator iter = m_collidables.begin(); iter != m_collidables.end(); iter++){
		(*iter)->Render();
	}
	for(SphObjectList::iterator iter = m_nonCollidables.begin(); iter != m_nonCollidables.end(); iter++){
		(*iter)->Render();
	}
	for(SphObjectList::iterator iter = m_items.begin(); iter != m_items.end(); iter++){
		(*iter)->Render();
	}
	this->m_explosion->Render();

}

//Update SphWorld.
//seconds - since last call.
void SphWorld::Update(float seconds){
	for(SphObjectList::iterator iter = m_collidables.begin(); iter != m_collidables.end();iter++){
		(*iter)->Update(seconds);
	}
	for(SphObjectList::iterator iter = m_nonCollidables.begin(); iter != m_nonCollidables.end();){
		SphObject* startingObj = *iter;
		(*iter)->Update(seconds);
		if(startingObj->GetObjType() == SphObject::PROJ_OBJ){
			if(!((SphProjectile*)startingObj)->IsAlive()){
				DELETE(startingObj);
				iter = m_nonCollidables.erase(iter);
				continue;
			}
		}
		iter++;
	}
	for(SphObjectList::iterator iter = m_items.begin(); iter != m_items.end();iter++){
		(*iter)->Update(seconds);
	}
	this->m_explosion->ComputeParticles(m_explosionPos);
}

//collisionsList - will be populated with collisions.
//bound - to collide with world objects.
void SphWorld::GetCollisions(SphCollisionsList* collisionsList, SphBound* bound, SphObjectList* objectList){
	for(SphObjectList::iterator iter = objectList->begin(); iter != objectList->end(); iter++){
		SphObject* object = *iter;
		if(object->m_bound == NULL || object->m_bound == bound) continue;
		int oldLength = collisionsList->size();
		Collide(collisionsList, bound, object->m_bound);
		
		//set object value for added collisions
		if(collisionsList->size() != oldLength){
			for(SphCollisionsList::iterator cIter = collisionsList->begin(); cIter != collisionsList->end(); cIter++){
				if((*cIter).m_object == NULL) (*cIter).m_object = object;
			}
		}
	}
}

// Generates a Particle Explosion
void SphWorld::GenerateExplosion(cfloat *position){
	Set(m_explosionPos,position);
	this->m_explosion->SetParameters(m_explosionPos);

}

//collisionsList - will be populated with collisions.
//bound - to collide with world objects.
void SphWorld::GetCollisions(SphCollisionsList* collisionsList, SphBound* bound){
	GetCollisions(collisionsList, bound, &m_collidables);
}

//collisionsList - will be populated with collisions.
//bound - to collide with world items.
void SphWorld::GetItemCollisions(SphCollisionsList* collisionsList, SphBound* bound){
	GetCollisions(collisionsList, bound, &m_items);
}

//object - to add to the world.
void SphWorld::AddCollidable(SphObject* object){
	m_collidables.push_front(object);
}

//object - to remove from the world.
void SphWorld::RemoveCollidable(SphObject* object){
	GenerateExplosion(object->GetPosition());
	if(object->GetObjType() == SphObject::ENEMY_OBJ)
		g_game.PlaySound("Audio/explosion2.ogg", 0.8f, false);
	m_collidables.remove(object);
}

//object - to add to the world.
void SphWorld::AddNonCollidable(SphObject* object){
	m_nonCollidables.push_front(object);
}

//object - to remove from the world.
void SphWorld::RemoveNonCollidable(SphObject* object){
	m_nonCollidables.remove(object);
}

//object - to add to the world.
void SphWorld::AddItem(SphObject* item){
	m_items.push_front(item);
}

//object - to remove from the world.
void SphWorld::RemoveItem(SphObject* item){
	m_items.remove(item);
}

//return - spawning position.
cfloat* SphWorld::GetSpawnPosition(){
	return m_startPos;
}

//Free all resources associated with SphWorld.
void SphWorld::Free(){
}