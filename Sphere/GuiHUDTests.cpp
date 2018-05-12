#pragma once
#include "TestHarness.h"
#include "SphInclude.h"
#include "SphOpenGL.h"
#include "SphMath.h"
#include "SphHUD.h"
#include "SphAvatar.h"
#include <stdio.h>

TEST(complete, HUD)
{
	printf("HUD Tests Done\n\n");
}
TEST(click_nothing, HUD)
{
	printf("Click No CFA Button - ");

	SphAvatar* test_avatar = new SphAvatar();
	SphHUD* test_hud = new SphHUD(test_avatar);

	int clicked = -1;
	clicked = test_hud->ClickCFAButton(0.5, 0.5);

	CHECK(clicked == -1)

	printf(" Done\n");


}
TEST(click_metal, HUD)
{
	printf("Click Metal CFA Button - ");

	SphAvatar* test_avatar = new SphAvatar();
	SphHUD* test_hud = new SphHUD(test_avatar);

	int clicked = -1;
	clicked = test_hud->ClickCFAButton(0.05f, 0.50f);

	CHECK(clicked == 3)

	printf(" Done\n");


}
TEST(click_rubber, HUD)
{
	printf("Click Rubber CFA Button - ");

	SphAvatar* test_avatar = new SphAvatar();
	SphHUD* test_hud = new SphHUD(test_avatar);

	int clicked = -1;
	clicked = test_hud->ClickCFAButton(0.05f, 0.60f);

	CHECK(clicked == 1)

	printf(" Done\n");


}
TEST(click_speed, HUD)
{
	printf("Click Speed CFA Button - ");

	SphAvatar* test_avatar = new SphAvatar();
	SphHUD* test_hud = new SphHUD(test_avatar);

	int clicked = -1;
	clicked = test_hud->ClickCFAButton(0.05f, 0.70f);

	CHECK(clicked == 2)

	printf(" Done\n");


}
TEST(click_normal, HUD)
{
	printf("Click Normal CFA Button - ");

	SphAvatar* test_avatar = new SphAvatar();
	SphHUD* test_hud = new SphHUD(test_avatar);

	int clicked = -1;
	clicked = test_hud->ClickCFAButton(0.05f, 0.80f);

	CHECK(clicked == 0)

	printf(" Done\n");


}
TEST(setup, HUD)
{
	printf("Initialize HUD Test - ");

	SphAvatar* test_avatar = new SphAvatar();
	SphHUD* test_hud = new SphHUD(test_avatar);

	CHECK(test_hud != NULL);

	printf("Done\n");
}
TEST(initialize, HUD)
{
	printf("Begin HUD Tests\n");
}
