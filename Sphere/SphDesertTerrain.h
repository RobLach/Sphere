/*
SphDesertTerrain.h, written by Joseph DiGiovanna, jdigiov2
*/
#pragma once
#include "SphInclude.h"
#include "SphTerrain.h"
#include "SphTree.h"
#include "SphTree.h"

class SphTree;
class SphTerrain;
class SphWorld;

//Our Desert Terrain class with unified, organized subroutines
class SphDesertTerrain : public SphTerrain {

public:

	//holds a set of trees to render
	SphTree *m_tree;
	int m_treeCount;

	SphDesertTerrain(cfloat* a_position, SphWorld* m_world);
	virtual void SphDesertTerrain::update(cfloat* a_position, cfloat* campos, cfloat* la, SphWorld* m_world);
	virtual void SphDesertTerrain::render(cfloat* pos, cfloat* la);
	virtual void SphDesertTerrain::m_renderGrass();
};