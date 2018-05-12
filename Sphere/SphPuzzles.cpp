#include <string.h>
#include "SphPuzzles.h"
#include "SphProp.h"
#include "SphTightBound.h"

SphPuzzles g_puzzles;

//Create SphPuzzles.
SphPuzzles::SphPuzzles(){
}

//Init SphPuzzles.
void SphPuzzles::Init(){
	m_puzzle1.Load("Meshes/Walls/", "Puzzle1.drkMesh");
	m_puzzle1a.Load("Meshes/Walls/", "Puzzle1a.drkMesh");
	m_puzzle2.Load("Meshes/Walls/", "Puzzle2.drkMesh");
	m_puzzle3.Load("Meshes/Walls/", "Puzzle3.drkMesh");
	m_puzzle4.Load("Meshes/Walls/", "Puzzle4.drkMesh");
	m_puzzle4a.Load("Meshes/Walls/", "Puzzle4a.drkMesh");
	m_puzzleD.Load("Meshes/Walls/", "PuzzleD.drkMesh");
	m_sphynx.Load("Meshes/Walls/", "Sphynx.drkMesh");
	m_ramp.Load("Meshes/Walls/", "Ramp.drkMesh");
	m_ankh.Load("Meshes/Walls/", "Ankh.drkMesh");
	m_basket.Load("Meshes/Walls/", "Basket.drkMesh");
	m_stairs.Load("Meshes/Walls/", "Stairs.drkMesh");
	m_pyramid.Load("Meshes/Walls/", "Pyramid.drkMesh");
	m_door.Load("Meshes/Walls/", "Door.drkMesh");
	m_wall2.Load("Meshes/Walls/", "Wall2.drkMesh");
	m_wall3.Load("Meshes/Walls/", "Wall3.drkMesh");
	m_wall45.Load("Meshes/Walls/", "Wall45.drkMesh");
	m_cloud1.Load("Meshes/Walls/", "Cloud1.drkMesh");
	m_cloud2.Load("Meshes/Walls/", "Cloud2.drkMesh");
	m_cloud3.Load("Meshes/Walls/", "Cloud3.drkMesh");
	m_cloud4.Load("Meshes/Walls/", "Cloud4.drkMesh");
	m_rainbow.Load("Meshes/Walls/", "Rainbow.drkMesh");
	m_alexei.Load("Meshes/Walls/", "Alexei.drkMesh");
	m_joe.Load("Meshes/Walls/", "Joe.drkMesh");
	m_kevin.Load("Meshes/Walls/", "Kevin.drkMesh");
	m_marc.Load("Meshes/Walls/", "Marc.drkMesh");
	m_mike.Load("Meshes/Walls/", "Mike.drkMesh");
	m_paul.Load("Meshes/Walls/", "Paul.drkMesh");
	m_rob.Load("Meshes/Walls/", "Rob.drkMesh");
}

//return - new prop based on the given mesh and with newly loaded bound.
SphProp* SphPuzzles::CreateProp(SphMesh* mesh, cchar* boundFilepath){
	SphProp* prop = new SphProp(mesh);
	if(boundFilepath != NULL){
		prop->m_bound = new SphTightBound(prop);
		((SphTightBound*)prop->m_bound)->Load(boundFilepath);
	}
	return prop;
}

//return - new prop based on the given name. NULL if incorrect name.
SphProp* SphPuzzles::CreateNewProp(cchar* name){
	if(!strcmp(name, "puzzle1")) return CreateProp(&m_puzzle1, "Meshes/Walls/Puzzle1.bound.drkMesh");
	if(!strcmp(name, "puzzle1a")) return CreateProp(&m_puzzle1a, "Meshes/Walls/Puzzle1a.bound.drkMesh");
	if(!strcmp(name, "puzzle2")) return CreateProp(&m_puzzle2, "Meshes/Walls/Puzzle2.bound.drkMesh");
	if(!strcmp(name, "puzzle3")) return CreateProp(&m_puzzle3, "Meshes/Walls/Puzzle3.bound.drkMesh");
	if(!strcmp(name, "puzzle4")) return CreateProp(&m_puzzle4, "Meshes/Walls/Puzzle4.bound.drkMesh");
	if(!strcmp(name, "puzzle4a")) return CreateProp(&m_puzzle4a, "Meshes/Walls/Puzzle4a.bound.drkMesh");
	if(!strcmp(name, "puzzleD")) return CreateProp(&m_puzzleD, "Meshes/Walls/PuzzleD.bound.drkMesh");
	if(!strcmp(name, "sphynx")) return CreateProp(&m_sphynx, "Meshes/Walls/Sphynx.bound.drkMesh");
	if(!strcmp(name, "ramp")) return CreateProp(&m_ramp, "Meshes/Walls/Ramp.bound.drkMesh");
	if(!strcmp(name, "ankh")) return CreateProp(&m_ankh, "Meshes/Walls/Ankh.bound.drkMesh");
	if(!strcmp(name, "basket")) return CreateProp(&m_basket, "Meshes/Walls/Basket.bound.drkMesh");
	if(!strcmp(name, "stairs")) return CreateProp(&m_stairs, "Meshes/Walls/Stairs.bound.drkMesh");
	if(!strcmp(name, "pyramid")) return CreateProp(&m_pyramid, "Meshes/Walls/Pyramid.bound.drkMesh");
	if(!strcmp(name, "door")) return CreateProp(&m_door, "Meshes/Walls/Door.bound.drkMesh");
	if(!strcmp(name, "wall2")) return CreateProp(&m_wall2, "Meshes/Walls/Wall2.bound.drkMesh");
	if(!strcmp(name, "wall3")) return CreateProp(&m_wall3, "Meshes/Walls/Wall3.bound.drkMesh");
	if(!strcmp(name, "wall45")) return CreateProp(&m_wall45, "Meshes/Walls/Wall45.bound.drkMesh");
	if(!strcmp(name, "cloud1")) return CreateProp(&m_cloud1, "Meshes/Walls/Cloud1.drkMesh");
	if(!strcmp(name, "cloud2")) return CreateProp(&m_cloud2, "Meshes/Walls/Cloud2.drkMesh");
	if(!strcmp(name, "cloud3")) return CreateProp(&m_cloud3, "Meshes/Walls/Cloud3.drkMesh");
	if(!strcmp(name, "cloud4")) return CreateProp(&m_cloud4, "Meshes/Walls/Cloud4.drkMesh");
	if(!strcmp(name, "rainbow")) return CreateProp(&m_rainbow, "Meshes/Walls/Rainbow.bound.drkMesh");
	if(!strcmp(name, "alexei")) return CreateProp(&m_alexei, NULL);
	if(!strcmp(name, "joe")) return CreateProp(&m_joe, NULL);
	if(!strcmp(name, "kevin")) return CreateProp(&m_kevin, NULL);
	if(!strcmp(name, "marc")) return CreateProp(&m_marc, NULL);
	if(!strcmp(name, "mike")) return CreateProp(&m_mike, NULL);
	if(!strcmp(name, "paul")) return CreateProp(&m_paul, NULL);
	if(!strcmp(name, "rob")) return CreateProp(&m_rob, NULL);
	return NULL;
}