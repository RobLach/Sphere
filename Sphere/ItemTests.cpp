#include "TestHarness.h"
#include "SphInclude.h"
#include "SphMath.h"
#include <stdio.h>

//"test game" data
#include "SphSdl.h"
#include "SphOpenAL.h"
#include "SphGame.h"
#include "SphWorld.h"
#include "SphHUD.h"

#include "SphAvatar.h"
//#include "SphEnemy.h" //-maybe add some of these
#include "SphKey.h"
#include "SphMedpack.h"

int initializeTestEnvironment()
{
	//these are required for the medpacks' "ItemPickup" method
	if(g_sdl.Init(0)) return -1;
	if(g_openAL.Init()) return -1;
	g_game.m_avatar = new SphAvatar();
	g_game.m_hud = new SphHUD(g_game.m_avatar);
	g_game.m_world = new SphWorld();

	return 0;
}

void closeTestEnvironment()
{
	g_game.Quit();
	g_openAL.Quit();
	g_sdl.Quit();
	delete(g_game.m_avatar);
	delete(g_game.m_hud);
	delete(g_game.m_world);
}

TEST(complete, Item)
{
	printf("---Item Tests Done\n");
	printf("-----------------------------------------\n\n");
}

TEST(smallHealthPack, Item)
{
	printf("-Running Item: \"Small Health Pack\" Tests - ");
	
	if(initializeTestEnvironment() == -1) FAIL("Initialization Failure");

	SphMedpack* smallMedpack = g_medpacks.CreateNewProp("small");
	int health;

	//initial health = 1
	health = 1;
	g_game.m_avatar->TakeDamage(100);
	g_game.m_avatar->ChangeHealth(health);
	CHECK(g_game.m_avatar->GetHealth() == health);
	smallMedpack->ItemPickup(g_game.m_avatar);
	health = Clamp(health+30, 0, 100);
	CHECK(g_game.m_avatar->GetHealth() == 31);

	//initial health = 100
	health = 100;
	g_game.m_avatar->TakeDamage(100);
	g_game.m_avatar->ChangeHealth(health);
	CHECK(g_game.m_avatar->GetHealth() == health);
	smallMedpack->ItemPickup(g_game.m_avatar);
	health = Clamp(health+30, 0, 100);
	CHECK(g_game.m_avatar->GetHealth() == health);

	//initial health = 30
	health = 30;
	g_game.m_avatar->TakeDamage(100);
	g_game.m_avatar->ChangeHealth(health);
	CHECK(g_game.m_avatar->GetHealth() == health);
	smallMedpack->ItemPickup(g_game.m_avatar);
	health = Clamp(health+30, 0, 100);
	CHECK(g_game.m_avatar->GetHealth() == health);

	delete(smallMedpack);

	closeTestEnvironment();

	printf(" DONE \n");
}

TEST(Item, mediumHealthPack)
{
	printf("-Running Item: \"Medium Health Pack\" Tests - ");
	initializeTestEnvironment();

	SphMedpack* mediumMedpack = g_medpacks.CreateNewProp("medium");
	int health;

	//initial health = 1
	health = 1;
	g_game.m_avatar->TakeDamage(100);
	g_game.m_avatar->ChangeHealth(health);
	CHECK(g_game.m_avatar->GetHealth() == health);
	mediumMedpack->ItemPickup(g_game.m_avatar);
	health = Clamp(health+60, 0, 100);
	CHECK(g_game.m_avatar->GetHealth() == health);

	//initial health = 100
	health = 100;
	g_game.m_avatar->TakeDamage(100);
	g_game.m_avatar->ChangeHealth(health);
	CHECK(g_game.m_avatar->GetHealth() == health);
	mediumMedpack->ItemPickup(g_game.m_avatar);
	health = Clamp(health+60, 0, 100);
	CHECK(g_game.m_avatar->GetHealth() == health);

	//initial health = 30
	health = 30;
	g_game.m_avatar->TakeDamage(100);
	g_game.m_avatar->ChangeHealth(health);
	CHECK(g_game.m_avatar->GetHealth() == health);
	mediumMedpack->ItemPickup(g_game.m_avatar);
	health = Clamp(health+60, 0, 100);
	CHECK(g_game.m_avatar->GetHealth() == health);

	delete(mediumMedpack);
	
	closeTestEnvironment();

	printf(" DONE \n");
}

TEST(fullHealthPack, Item)
{
	printf("-Running Item: \"Full Health Pack\" Tests - ");
	initializeTestEnvironment();

	SphMedpack* fullMedpack = g_medpacks.CreateNewProp("full");	
	int health;

	//initial health = 1
	health = 1;
	g_game.m_avatar->TakeDamage(100);
	g_game.m_avatar->ChangeHealth(health);
	CHECK(g_game.m_avatar->GetHealth() == health);
	fullMedpack->ItemPickup(g_game.m_avatar);
	health = Clamp(health+100, 0, 100);
	CHECK(g_game.m_avatar->GetHealth() == health);

	//initial health = 100
	g_game.m_avatar->TakeDamage(100);
	g_game.m_avatar->ChangeHealth(health);
	CHECK(g_game.m_avatar->GetHealth() == health);
	fullMedpack->ItemPickup(g_game.m_avatar);
	health = Clamp(health+100, 0, 100);
	CHECK(g_game.m_avatar->GetHealth() == health);

	//initial health = 30
	health = 30;
	g_game.m_avatar->TakeDamage(100);
	g_game.m_avatar->ChangeHealth(health);
	CHECK(g_game.m_avatar->GetHealth() == health);
	fullMedpack->ItemPickup(g_game.m_avatar);
	health = Clamp(health+100, 0, 100);
	CHECK(g_game.m_avatar->GetHealth() == health);
	delete(fullMedpack);
	
	closeTestEnvironment();

	printf(" DONE \n");
}

TEST(keys, Item)
{
	printf("-Running Item: \"Key\" Tests - ");
	initializeTestEnvironment();

	SphKey* redKey = g_keys.CreateNewProp("red");

	//check if picking up the key adds it to the avatar's keychain
	CHECK(!g_game.m_avatar->HasKey(SphKey::Red));
	redKey->ItemPickup(g_game.m_avatar);
	CHECK(g_game.m_avatar->HasKey(SphKey::Red));

	delete(redKey);
	
	closeTestEnvironment();

	printf(" DONE \n");
}

TEST(RubberCFA, Avatar)
{
	printf("-Running Item: \"Rubber CFA\" Tests - ");
	initializeTestEnvironment();

	SphItem* item = SphItem::CreateNewProp("rubber");

	//check if picking up the rubber cfa adds it to the avatar
	CHECK(!g_game.m_avatar->HasClosedType(SphAvatar::Rubber));
	g_game.m_avatar->AddClosedType(SphAvatar::Rubber);
	CHECK(g_game.m_avatar->HasClosedType(SphAvatar::Rubber));

	delete(item);
	closeTestEnvironment();

	printf(" DONE \n");
}

TEST(MetalCFA, Avatar)
{
	printf("-Running Item: \"Metal CFA\" Tests - ");
	initializeTestEnvironment();

	SphItem* item = SphItem::CreateNewProp("metal");

	//check if picking up the metal cfa adds it to the avatar
	CHECK(!g_game.m_avatar->HasClosedType(SphAvatar::Metal));
	g_game.m_avatar->AddClosedType(SphAvatar::Metal);
	CHECK(g_game.m_avatar->HasClosedType(SphAvatar::Metal));

	delete(item);
	closeTestEnvironment();

	printf(" DONE \n");
}

TEST(SpeedCFA, Avatar)
{
	printf("-Running Item: \"Speed CFA\" Tests - ");
	initializeTestEnvironment();

	SphItem* item = SphItem::CreateNewProp("speed");

	//check if picking up the speed cfa adds it to the avatar
	CHECK(!g_game.m_avatar->HasClosedType(SphAvatar::Speed));
	g_game.m_avatar->AddClosedType(SphAvatar::Speed);
	CHECK(g_game.m_avatar->HasClosedType(SphAvatar::Speed));

	delete(item);
	closeTestEnvironment();

	printf(" DONE \n");
}

TEST(LaserOFA, Avatar)
{
	printf("-Running Item: \"Laser OFA\" Tests - ");
	initializeTestEnvironment();

	SphItem* item = SphItem::CreateNewProp("laser");

	//check if picking up the laser ofa adds it to the avatar
	CHECK(!g_game.m_avatar->HasAttachment(SphAvatar::LaserGun));
	g_game.m_avatar->AddAttachment(SphAvatar::LaserGun);
	CHECK(g_game.m_avatar->HasAttachment(SphAvatar::LaserGun));

	delete(item);
	closeTestEnvironment();

	printf(" DONE \n");
}

TEST(SprayOFA, Avatar)
{
	printf("-Running Item: \"Spray OFA\" Tests - ");
	initializeTestEnvironment();

	SphItem* item = SphItem::CreateNewProp("spray");

	//check if picking up the spray ofa adds it to the avatar
	CHECK(!g_game.m_avatar->HasAttachment(SphAvatar::SprayGun));
	g_game.m_avatar->AddAttachment(SphAvatar::SprayGun);
	CHECK(g_game.m_avatar->HasAttachment(SphAvatar::SprayGun));

	delete(item);
	closeTestEnvironment();

	printf(" DONE \n");
}

TEST(LegsOFA, Avatar)
{
	printf("-Running Item: \"Legs OFA\" Tests - ");
	initializeTestEnvironment();

	SphItem* item = SphItem::CreateNewProp("legs");

	//check if picking up the legs ofa adds it to the avatar
	CHECK(!g_game.m_avatar->HasAttachment(SphAvatar::Legs));
	g_game.m_avatar->AddAttachment(SphAvatar::Legs);
	CHECK(g_game.m_avatar->HasAttachment(SphAvatar::Legs));

	delete(item);
	closeTestEnvironment();

	printf(" DONE \n");
}

TEST(begin, Item)
{
	printf("\n-----------------------------------------\n");
	printf("---Starting Item Tests...\n");
}