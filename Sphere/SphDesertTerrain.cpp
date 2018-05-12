/*
SphDesertTerrain, for ease of use within SphGame.
Written by Joseph DiGiovanna, jdigiov2
*/
#pragma once
#include "SphMath.h"
#include "SphDesertTerrain.h"
#include "SphWorld.h"
#include "SphTree.h"

//Create SphGround.
SphDesertTerrain::SphDesertTerrain(cfloat* a_position, SphWorld* m_world) 
							: SphTerrain("Textures/Landscape_Textures/heightmap.tga"){
	int curXloc, curZloc;
	float avX, avY, avZ;
	float xstart, ystart, zstart;
	float xScale, yScale, zScale;
	float m_innerRadius, m_outerRadius, m_patches, m_minimumHeight;
	m_innerRadius = 90;
	m_outerRadius = 100;
	m_patches = 120;
	m_minimumHeight = 15;


	//run our basic terrain constructor. (changed this to a derived class, see constructor)
	//m_terrain = new SphTerrain("Textures/Landscape_Textures/heightmap.tga");

	//this sets a 16 unit gap between verticies used from the heightmap (16 or 8 are best, 4 or less hurts framerate)
	setStepSize(16);

	//sets it so that our terrain is centered roughly around (0, 0)
	setStartingCoordinates(-50, 0, -50);

	//sets our terrains x, y and z scales
	setTerrainScale(.2f, .06f, .2f);

	//This step activates bezier curves to smooth the landscape
	//in our case, we use 64X64 unit blocks at 4X4 control points
	//each.  This lets us smooth over our terrain on a general scale
	//while still maintaining height values similar too the original
	setBezierActive(64, 4); 

	//this line smooths over the edges of the terrain (basically makes
	//the extremeties look nicer by averaging them out)
	//smoothTerrainEdges(16);

	//This line creates a flat plane with smooth edges around the 1st puzzle
	//createFlatPlaneSmoothEdges(0, 425, 450, 150, 50, 90);

	//This line creates a flat plane with smooth edges around the 2nd puzzle
	//createFlatPlaneSmoothEdges(450, 425, 150, 150, 50, 0);

	//This line creates a flat plane with smooth edges around the 3rd puzzle
	//createFlatPlaneSmoothEdges(660, 350, 100, 300, 50, 0);

	//This line creates a flat plane with smooth edges around the 4th puzzle
	//createFlatPlaneSmoothEdges(400, 750, 200, 100, 50, 0);

	//We calculate all triangle normals for pre-computed backface culling and lighting
	calculateNormals();

	//We calculate all the boundaries for the terrain chunks and save them 
	//for efficient remove and load.
	calculateBoundaries();

	//This activates detail textures on our terrain, lets us define what 
	//texture to use, and sets the number of tiles for our terrain
	setDetailTexture(true, "Textures/Landscape_Textures/detail.tga", 16);

	//this line sets the texture of our terrain itself
	//setTexture("Textures/Landscape_Textures/map_farview.tga");
	setTexture("Textures/Landscape_Textures/Ground.tga");

	//Sets the rendermode to solid
	setRenderMode(0);

	//sets the number of texture tiles to 8 for our base texture
	setTiles(8);

	//get our avatars x, y and z, and our landscapes, start / scale values.
	avX = a_position[0]; avY = a_position[1]; avZ = a_position[2];
	xScale = getScale()[0]; yScale = getScale()[1]; zScale = getScale()[2];
	xstart = getStartingCoordinates()[0]; ystart = getStartingCoordinates()[1]; zstart = getStartingCoordinates()[2];

	//make sure we're looking at a point on the actual landscape and find that exact chunck
	curXloc = (int)((avX / xScale) - xstart);
	curZloc = (int)((avZ / zScale) - zstart);
	curXloc = curXloc / getStepSize();
	curZloc = curZloc / getStepSize();

	//make sure it's not out of bounds
	if (curXloc < 1) curXloc = 1;
	if (curZloc < 1) curZloc = 1;
	
	//add our collidable chuncks to the world.
	m_world->AddCollidable(&m_boundaries[curXloc][curZloc]);
	m_world->AddCollidable(&m_boundaries[curXloc + 1][curZloc]);
	m_world->AddCollidable(&m_boundaries[curXloc - 1][curZloc]);
	m_world->AddCollidable(&m_boundaries[curXloc][curZloc + 1]);
	m_world->AddCollidable(&m_boundaries[curXloc][curZloc - 1]);
	
	//update out boundaries stored in the terrain
	setBoundaries(curXloc, curZloc);
	initializeGrass(0, 0, m_innerRadius, m_outerRadius, m_patches, m_minimumHeight);
	loadGrassTexture("Textures/Landscape_Textures/grass2.tga");

	//This Code sets up our trees randomly within 3 given safe zones.
	m_treeCount = 18;
	m_tree = new SphTree[m_treeCount]();

	for (int i = 0; i < m_treeCount; i++){
		if (i <= 6){
			m_tree[i].setInformation(60.0f + ((double)rand() / (double)(RAND_MAX)) * 33, 15.2f, 135.0f + ((double)rand() / (double)(RAND_MAX)) * 20, 3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.05f, 0.025f, 0.275f - ((double)rand() / (double)(RAND_MAX)) * 0.1, m_world);
		}else if (i < 12 && i > 6){
			m_tree[i].setInformation(98.0f + ((double)rand() / (double)(RAND_MAX)) * 28, 15.2f, 110.0f + ((double)rand() / (double)(RAND_MAX)) * 50, 3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.05f, 0.025f, 0.275f - ((double)rand() / (double)(RAND_MAX)) * 0.1, m_world);
		}else{
			m_tree[i].setInformation(150.0f + ((double)rand() / (double)(RAND_MAX)) * 18, 15.2f, 110.0f + ((double)rand() / (double)(RAND_MAX)) * 50, 3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.05f, 0.025f, 0.275f - ((double)rand() / (double)(RAND_MAX)) * 0.1, m_world);
		}
		m_tree[i].init();
	}
}

// Update Terrain.
void SphDesertTerrain::update(cfloat* a_position, cfloat* campos, cfloat* la, SphWorld* m_world){

	int curXloc, curZloc;  //our avatars current x and z location on the grid
	float xScale, yScale, zScale, xstart, ystart, zstart;  //our terrains scale and start positions
	float avX, avY, avZ;  //our avatars x y and z positions for easy access

	//get our avatars x, y and z, and our landscapes, start / scale values.
	avX = a_position[0]; avY = a_position[1]; avZ = a_position[2];
	xScale = getScale()[0]; yScale = getScale()[1]; zScale = getScale()[2];
	xstart = getStartingCoordinates()[0]; ystart = getStartingCoordinates()[1]; zstart = getStartingCoordinates()[2];

	//make sure we're looking at a point on the actual landscape and find that exact chunck
	curXloc = (int)((avX / xScale) - xstart);
	curZloc = (int)((avZ / zScale) - zstart);
	curXloc = curXloc / getStepSize();
	curZloc = curZloc / getStepSize();

	//make sure we're within the boundaries
	if (curXloc < 1) curXloc = 1;
	if (curZloc < 1) curZloc = 1;

	updateGrass(campos, a_position, la);
	//m_tree->growBranch(m_tree->tree, 0.1);

	//remove old and add the new ones in if we've moved and need to update
	if (curXloc != getBoundaries()[0] || curZloc != getBoundaries()[1]){
		m_world->RemoveCollidable(&m_boundaries[getBoundaries()[0]][getBoundaries()[1]]);		
		m_world->RemoveCollidable(&m_boundaries[getBoundaries()[0] - 1][getBoundaries()[1]]);
		m_world->RemoveCollidable(&m_boundaries[getBoundaries()[0] + 1][getBoundaries()[1]]);
		m_world->RemoveCollidable(&m_boundaries[getBoundaries()[0]][getBoundaries()[1] - 1]);
		m_world->RemoveCollidable(&m_boundaries[getBoundaries()[0]][getBoundaries()[1] + 1]);

		m_world->AddCollidable(&m_boundaries[curXloc][curZloc]);
		m_world->AddCollidable(&m_boundaries[curXloc + 1][curZloc]);
		m_world->AddCollidable(&m_boundaries[curXloc - 1][curZloc]);
		m_world->AddCollidable(&m_boundaries[curXloc][curZloc + 1]);
		m_world->AddCollidable(&m_boundaries[curXloc][curZloc - 1]);

		//save the new active boundaries
		setBoundaries(curXloc, curZloc);
	}
}

//render the terrain itself.
void SphDesertTerrain::render(cfloat* pos, cfloat* la){
	for (int i = 0; i < m_treeCount; i++){
		m_tree[i].renderTree(pos, la);
	}

	renderTerrain(pos[0], pos[1], pos[2], 0, 40.1f);
	
}

void SphDesertTerrain::m_renderGrass(){
	renderGrass();
}