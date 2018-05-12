#include "SphMath.h"
#include "SphObject.h"
#include "SphLevel.h"

SphLevel::SphLevel(){

	LoadLevel(); //TODO:create some level format to be passed to LoadLevel;

}

void SphLevel::Render(){

	for(SphObjectList::iterator iter = m_objects.begin(); iter != m_objects.end(); iter++){
		(*iter)->Render();
	}

	//Render the entities (Do we need to do this or are they all in Objects?)
	//for(SphEntityList::iterator iter = m_objects.begin(); iter != m_objects.end(); iter++){
	//	(*iter)->Render();
	//}

	//Render Terrain - Insert code here

}

void SphLevel::Update(float seconds){
	for(SphObjectList::iterator iter = m_objects.begin(); iter != m_objects.end(); iter++){
		(*iter)->Update(seconds);
	}
}

void SphLevel::AddObject(SphObject* object){
	m_objects.push_front(object);
}

void SphLevel::RemoveObject(SphObject* object){
	m_objects.remove(object);
}

void SphLevel::Free(){
}