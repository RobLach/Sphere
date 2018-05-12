#pragma once
#include "TestHarness.h"
#include "SphInclude.h"
#include "SphOpenGL.h"
#include "SphObject.h"
#include "SphBound.h"
#include "SphSphericBound.h"
#include "SphCollisionDetection.h"
#include "SphMath.h"
#include <stdio.h>

TEST(complete, Sphere_SphereCollision)
{
	printf("Sphere-Sphere Collision Tests Done\n\n");
}

TEST(full_miss, Sphere_SphereCollision)
{
	printf("Running Sphere-Sphere Full Miss Collision Test - ");

	SphObject *o1 = new SphObject();
	SphObject *o2 = new SphObject();
	SphCollisionsList info;

	o1->m_bound = new SphSphericBound(o1, 1.0f);
	o2->m_bound = new SphSphericBound(o2, 1.0f);

	o1->SetPosition(0.0f, 3.0f, 5.0f);
	o2->SetPosition(0.0f, 0.0f, 0.0f);

	float seconds = 0.0;

	while(seconds < 10.0)
	{
		seconds += 0.1f;
		o1->Translate3(0.0f, 0.0f, -0.1f);
		Collide(&info, o1->m_bound, o2->m_bound);
	}
	CHECK(info.empty());

	printf("Done\n");
}

TEST(near_miss, Sphere_SphereCollision)
{
	printf("Running Sphere-Sphere Near Miss Collision Test - ");

	SphObject *o1 = new SphObject();
	SphObject *o2 = new SphObject();
	SphCollisionsList info;

	o1->m_bound = new SphSphericBound(o1, 1.0f);
	o2->m_bound = new SphSphericBound(o2, 1.0f);

	o1->SetPosition(0.0f, 2.01f, 5.0f);
	o2->SetPosition(0.0f, 0.0f, 0.0f);

	float seconds = 0.0;

	while(seconds < 10.0)
	{
		seconds += 0.1f;
		o1->Translate3(0.0f, 0.0f, -0.1f);
		Collide(&info, o1->m_bound, o2->m_bound);
	}
	CHECK(info.empty());

	printf("Done\n");
}

TEST(near_hit, Sphere_SphereCollision)
{
	printf("Running Sphere-Sphere Near Hit Collision Test - ");

	SphObject *o1 = new SphObject();
	SphObject *o2 = new SphObject();
	SphCollisionsList info;

	o1->m_bound = new SphSphericBound(o1, 1.0f);
	o2->m_bound = new SphSphericBound(o2, 1.0f);

	o1->SetPosition(0.0f, 2.0f, 5.0f);
	o2->SetPosition(0.0f, 0.0f, 0.0f);

	float seconds = 0.0;

	while(seconds < 10.0)
	{
		seconds += 0.1f;
		o1->Translate3(0.0f, 0.0f, -0.1f);
		Collide(&info, o1->m_bound, o2->m_bound);
	}
	CHECK(!info.empty());

	printf("Done\n");
}

TEST(full_hit, Sphere_SphereCollision)
{
	printf("Running Sphere-Sphere Full Hit Collision Test - ");

	SphObject *o1 = new SphObject();
	SphObject *o2 = new SphObject();
	SphCollisionsList info;

	o1->m_bound = new SphSphericBound(o1, 1.0f);
	o2->m_bound = new SphSphericBound(o2, 1.0f);

	o1->SetPosition(0.0f, 0.0f, 5.0f);
	o2->SetPosition(0.0f, 0.0f, 0.0f);

	float seconds = 0.0;

	while(seconds < 10.0)
	{
		seconds += 0.1f;
		o1->Translate3(0.0f, 0.0f, -0.1f);
		Collide(&info, o1->m_bound, o2->m_bound);
	}
	CHECK(!info.empty());

	printf("Done\n");
}

TEST(initialize, Sphere_SphereCollision)
{
	printf("Begin Sphere-Sphere Collision Tests\n");
}
