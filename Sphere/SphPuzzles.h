#pragma once
#include "SphInclude.h"
#include "SphMesh.h"

class SphProp;

//SphPuzzles creates SphProps and loads appropriates meshes for them.
class SphPuzzles {

protected:
	SphMesh m_puzzle1, m_puzzle1a, m_puzzle2, m_puzzle3, m_puzzle4, m_puzzle4a, m_puzzleD, m_sphynx, m_ramp, m_ankh, m_basket, m_stairs, m_pyramid, m_door;
	SphMesh m_wall2, m_wall3, m_wall45, m_cloud1, m_cloud2, m_cloud3, m_cloud4, m_rainbow;
	SphMesh m_alexei, m_joe, m_kevin, m_marc, m_mike, m_paul, m_rob;

	SphProp* CreateProp(SphMesh* mesh, cchar* boundFilepath);

public:
	SphPuzzles();
	void Init();
	SphProp* CreateNewProp(cchar* name);
};

extern SphPuzzles g_puzzles;