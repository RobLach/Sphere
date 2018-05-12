#include "SphItem.h"
#include "SphAbilityItem.h"
#include "SphInclude.h"
#include "SphMedpack.h"
#include "SphCactus.h"
#include "SphMesh.h"
#include "SphAvatar.h"
#include "SphOpenAL.h"
#include "SphGame.h"
#include "SphWorld.h"
#include "SphSphericBound.h"

//Create SphItem
SphItem::SphItem(){
}

//return this object's object type
SphObject::SphObjectType SphItem::GetObjType(){
	return ITEM_OBJ;
}

SphItem* SphItem::CreateNewProp(cchar* name){
	if(!strcmp(name, "small") || !strcmp(name, "medium") || !strcmp(name, "full")) {
		return g_medpacks.CreateNewProp(name);	
	}
	else if(!strcmp(name, "red") || !strcmp(name, "orange") || !strcmp(name, "yellow") || !strcmp(name, "green") ||
		    !strcmp(name, "blue") || !strcmp(name, "indigo") || !strcmp(name, "violet")) {
		return g_keys.CreateNewProp(name);	
	}
	else if(!strcmp(name, "laser") || !strcmp(name, "spray") || !strcmp(name, "legs") ||
	    !strcmp(name, "speed") || !strcmp(name, "metal") || !strcmp(name, "rubber")) {
		return g_abilityItems.CreateNewProp(name);	
	}
	else if(!strcmp(name, "cactus")) {
		return g_cacti.CreateNewProp(name);	
	}
	else return (SphItem*)(0);
}