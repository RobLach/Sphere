#include "SphMath.h"
#include "SphDesertWorld.h"
#include "SphProp.h"
#include "SphPuzzles.h"
#include "SphMedpack.h"
#include "SphCubeEnemy.h"
#include "SphSwarmEnemy.h"
#include "SphAnubisBoss.h"
#include "SphLight.h"
#include "SphGame.h"
#include "SphAvatar.h"
#include "SphKey.h"
#include "SphItem.h"
#include "SphAbilityItem.h"
#include "SphQuicksand.h"
#include "SphSphericBound.h"
#include "SphMovingPlatform.h"
#include "SphWallPuzzle.h"
#include "SphCrate.h"
#include "SphTeleport.h"
#include "SphCactus.h"

const float groundLevel = 15.0f;

//Create SphDesertWorld.
SphDesertWorld::SphDesertWorld(){
	g_puzzles.Init();
	g_medpacks.Init();
	g_keys.Init();
	g_cacti.Init();
	g_abilityItems.Init();
	g_movingPlatforms.Init();
	AddPuzzles();
	AddItems();
	AddEnemies();
	AddOther();
	AddLights();
	Set(m_startPos, 12.0f, 30.0f, 5.0f);
}

//Update SphDesertWorld.
//seconds - since the last call.
void SphDesertWorld::Update(float seconds){	
	if(m_yellowDoor != NULL && g_game.m_avatar->HasKey(SphKey::Yellow)){
		if(Distance(g_game.m_avatar->GetPosition(), m_yellowDoor->GetPosition()) < 16.0f){
			RemoveCollidable(m_yellowDoor);
			DELETE(m_yellowDoor);
		}
	}
	float destination[N_XYZ];
	Set(destination, 56.7f, 95.0f, 235.6f);
	if(Distance(g_game.m_avatar->GetPosition(), destination) < 9.0f){
		printf("DONE!!!\n");
		//level is finished!
	}
	SphWorld::Update(seconds);
}

//Add puzzles.
void SphDesertWorld::AddPuzzles(){
	float rot[N_QUAT];
	SphProp* prop;

	AddMazePuzzle(this, 40.0f - 50.0f, groundLevel - 2.5f, 8.0f - 50.0f);

	#define ADD_PUZZLE(puzzle, x, y, z, deg)		\
		prop = g_puzzles.CreateNewProp(puzzle);	\
		prop->SetPosition(x, y, z);					\
		QuaternionY(rot, DegToRad(deg));				\
		prop->SetRotation(rot);							\
		AddCollidable(prop);

	ADD_PUZZLE("puzzle1", 12.0f, groundLevel - 4.0f, 31.0f, 90.0f);
	ADD_PUZZLE("puzzle1a", 120.0f, groundLevel - 4.0f, 44.0f, 90.0f);
	ADD_PUZZLE("puzzle2", 11.0f, groundLevel + 13.0f, 83.0f, 180.0f);
	ADD_PUZZLE("puzzle3", 145.0f + 20.0f, groundLevel + 17.3f, 10.0f - 1.8f, 180.0f);
	ADD_PUZZLE("puzzleD", 85.0f, groundLevel + 10.0f, 55.0f, 90.0f);
	ADD_PUZZLE("puzzle4", 170.0f + 15.5f, groundLevel + 10.0f, 100.0f - 9.3f, 180.0f);
	
	ADD_PUZZLE("sphynx", 85.0f, groundLevel + 12.0f, 110.0f + 3.0f, 270.0f);
	ADD_PUZZLE("ramp", 11.0f, groundLevel + 7.5f, 160.0f, 180.0f);
	ADD_PUZZLE("ankh", 30.0f, groundLevel + 12.9f, 160.0f, 180.0f);
	ADD_PUZZLE("basket", 45.0f, groundLevel + 11.0f, 160.0f, 180.0f);

	ADD_PUZZLE("ramp", 140.0f, groundLevel + 7.5f, 110.0f + 2.0f, 90.0f);
	ADD_PUZZLE("ankh", 140.0f - 3.0f, groundLevel + 12.9f, 130.0f, 90.0f);
	ADD_PUZZLE("stairs", 140.0f - 6.0f, groundLevel + 10.0f, 159.0f, 90.0f);
	ADD_PUZZLE("pyramid", 93.0f, groundLevel, 273.0f, 180.0f);
	ADD_PUZZLE("door", 56.7f, 97.0f, 235.6f, 180.0f);
	m_yellowDoor = prop;

	ADD_PUZZLE("cloud4", 230.0f, 22.0f,  81.0f, 90.0f);
	ADD_PUZZLE("cloud4", 225.0f, 22.0f,  61.0f, 90.0f);
	ADD_PUZZLE("cloud4", 235.0f, 22.0f,  41.0f, 90.0f);
	ADD_PUZZLE("cloud4", 225.0f, 22.0f,  31.0f, 90.0f);
	ADD_PUZZLE("cloud4", 230.0f, 22.0f,  21.0f, 90.0f);
	ADD_PUZZLE("cloud4", 235.0f, 22.0f,  11.0f, 90.0f);
	ADD_PUZZLE("cloud3", 200.0f, 22.0f, -40.0f, 180.0f);
	ADD_PUZZLE("cloud1", 130.0f, 22.0f, -40.0f, 180.0f);
	ADD_PUZZLE("cloud2",  60.0f, 22.0f, -40.0f, 180.0f);

	//rainbow road level
	ADD_PUZZLE("rainbow", 12.0f, 130.0f, 5.0f, 6.0f);//change starting position to this position
	ADD_PUZZLE("rob", 22.0f, 131.0f, 4.0f, 270.0f);
	ADD_PUZZLE("marc", 36.0f, 132.0f, 5.0f, 270.0f);
	ADD_PUZZLE("joe", 50.0f, 133.0f, 11.0f, 270.0f);
	ADD_PUZZLE("mike", 72.0f, 131.0f, 15.0f, 270.0f);
	ADD_PUZZLE("kevin", 71.0f, 130.0f, -13.0f, 90.0f);
	ADD_PUZZLE("paul", 51.0f, 129.0f, -17.0f, 90.0f);
	ADD_PUZZLE("alexei", 15.0f, 120.0f, -7.0f, 90.0f);

	#undef ADD_PUZZLE
}

//Add items.
void SphDesertWorld::AddItems(){
	SphItem* item;

	//temp
	item = SphItem::CreateNewProp("yellow");
	item->SetPosition(56.7f, 102.0f, 235.6f);
	AddItem(item);

	item = SphItem::CreateNewProp("red");
	item->SetPosition(50.0f, groundLevel + 0.5f, 14.0f);
	AddItem(item);

	item = SphItem::CreateNewProp("rubber");
	item->SetPosition(6.0f, groundLevel + 0.5f, 4.0f);
	AddItem(item);

	//temp
	//item = SphItem::CreateNewProp("speed");
	//item->SetPosition(/*6.0f*/8.0f, groundLevel + 0.5f, 4.0f);
	//AddItem(item);
	//item = SphItem::CreateNewProp("rubber");
	//item->SetPosition(/*6.0f*/8.0f, groundLevel + 0.5f, 4.0f);
	//AddItem(item);

	item = SphItem::CreateNewProp("laser");
	item->SetPosition(11.0f, 41.5f, 104.5f);
	AddItem(item);

	item = SphItem::CreateNewProp("spray");
	item->SetPosition(200.0f, 48.0f, 4.4f);
	AddItem(item);

	item = SphItem::CreateNewProp("full");
	item->SetPosition(175.0f, 16.0f, 4.0f);
	AddItem(item);

	item = SphItem::CreateNewProp("legs");
	item->SetPosition(84.7f, 37.0f, 55.0f);
	AddItem(item);

	item = SphItem::CreateNewProp("speed");
	item->SetPosition(85.0f, 30.0f, 110.0f);
	AddItem(item);

	item = SphItem::CreateNewProp("small");
	item->SetPosition(85.0f, 29.5f, 114.0f);
	AddItem(item);

	item = SphItem::CreateNewProp("small");
	item->SetPosition(85.0f, 29.5f, 106.0f);
	AddItem(item);

	item = SphItem::CreateNewProp("metal");
	item->SetPosition(44.0f, 17.3f, 160.0f);
	AddItem(item);

	// Cactus collectables
	item = SphItem::CreateNewProp("cactus");
	item->SetPosition(44.0f, groundLevel + 0.5f, 14.0f);
	AddItem(item);
	item = SphItem::CreateNewProp("cactus");
	item->SetPosition(47.92f, 37.00f, 155.9f);
	AddItem(item);
	item = SphItem::CreateNewProp("cactus");
	item->SetPosition(84.83f, 25.20f, 138.62f);
	AddItem(item);
	item = SphItem::CreateNewProp("cactus");
	item->SetPosition(156.32f, 45.56f, 7.20f);
	AddItem(item);
	item = SphItem::CreateNewProp("cactus");
	item->SetPosition(75.2f,  14.48f, 36.4f);
	AddItem(item);
	item = SphItem::CreateNewProp("cactus");
	item->SetPosition(189.13f, 15.30f, 82.05f);
	AddItem(item);
	item = SphItem::CreateNewProp("cactus");
	item->SetPosition(189.83f, 15.30f, 117.60f);
	AddItem(item);
	// End cactus collectables.

}

//Add enemies.
void SphDesertWorld::AddEnemies(){
	SphCubeEnemy* cubeEnemy;
	
	#define ADD_CUBE(x, y, z)					\
		cubeEnemy = new SphCubeEnemy();		\
		cubeEnemy->SetPosition(x, y, z);		\
		AddCollidable(cubeEnemy);

	ADD_CUBE(42.0f, groundLevel + 1.0f, 61.0f);
	ADD_CUBE(41.0f, groundLevel + 1.0f, 65.0f);

	//pyramid guards
	ADD_CUBE(180.0f, 36.0f + 1.0f, 187.0f);
	ADD_CUBE(120.0f, 36.0f + 1.0f, 187.0f);
	ADD_CUBE( 60.0f, 36.0f + 1.0f, 187.0f);
	ADD_CUBE(  6.0f, 36.0f + 1.0f, 187.0f);
	ADD_CUBE(180.0f, 36.0f + 1.0f, 250.0f);
	ADD_CUBE(180.0f, 36.0f + 1.0f, 310.0f);
	ADD_CUBE(180.0f, 36.0f + 1.0f, 360.0f);
	//end pyramid guards

	SphSwarmEnemy* swarmEnemy = new SphSwarmEnemy();
	swarmEnemy->SetPosition(124.0f, 16.0f, 3.0f);
	AddCollidable(swarmEnemy);

	swarmEnemy = new SphSwarmEnemy();
	swarmEnemy->SetPosition(95.0f, groundLevel, 110.0f);
	AddCollidable(swarmEnemy);

	swarmEnemy = new SphSwarmEnemy();
	swarmEnemy->SetPosition(75.0f, groundLevel, 130.0f);
	AddCollidable(swarmEnemy);

	SphAnubisBoss* anubis = new SphAnubisBoss();
	anubis->SetPosition(115.0f, 100.0f, 250.0f);
	AddCollidable(anubis);
}

//Add other objects.
void SphDesertWorld::AddOther(){
	float rot[N_QUAT];
	
	SphCrate* crate = new SphCrate();
	QuaternionY(rot, PId2);
	crate->Rotate(rot);
	crate->SetPosition(6.0f, groundLevel - 0.4f, 4.0f);
	AddCollidable(crate);

	//this platform is triggered by collision with the avatar, and only moves one way (does not come back)
	float endPoints[] = {230.0f, 10.0f, 101.0f,		230.0f, 10.0f, -40.0f,		0.0f, 10.0f, -40.0f,      0.0f, 30.0f, -40.0f };
	SphMovingPlatform* platform = g_movingPlatforms.CreateNewProp("triggered_oneway", 0.0f, 5.0f, 4, endPoints);
	QuaternionY(rot, PId2);
	platform->SetRotation(rot);
	AddCollidable(platform);

	SphTeleport* teleport = new SphTeleport(85.0f, 40.0f, 115.0f);
	teleport->SetPosition(0.0f, 30.0f, -40.0f);
	AddNonCollidable(teleport);
	
	SphProp* quickSand = new SphQuicksand();
	AddNonCollidable(quickSand);

	//teleport for rainbow level
	teleport = new SphTeleport(12.0f, 130.0f, 5.0f);
	teleport->SetPosition(58.0f, 111.0f, 20.0f);
	AddNonCollidable(teleport);
	//teleport to rainbow level (rainbow entrance)
	teleport = new SphTeleport(12.0f, 130.0f, 5.0f);
	teleport->SetPosition(56.7f, 96.0f, 235.6f);
	AddNonCollidable(teleport);

}

//Add lights.
void SphDesertWorld::AddLights(){
	float direction[N_XYZ];
	Set(direction, 1.0f, 1.0f, 0.01f);
	SphLight* light = new SphLight();
	light->SetPosition(direction);
	light->Render();
	AddNonCollidable(light);
}