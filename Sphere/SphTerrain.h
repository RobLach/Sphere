/*
SphTerrain.h, created by Joseph DiGiovanna
class created with the intention of simplifying
the loading, texturing and renduring of terrain.
*/
#pragma once
#include "SphInclude.h"
#include "SphObject.h"


typedef struct point_3d {						// Structure For A 3-Dimensional Point 
	float x, y, z;
} POINT_3D;

typedef struct bpatch {							// Structure For A 3rd Degree Bezier Patch 
	POINT_3D	anchors[4][4];					// 4x4 Grid Of Anchor Points
	uint		dlBPatch;						// Display List For Bezier Patch
	uint		texture;						// Texture For The Patch
} BEZIER_PATCH;

typedef struct grassPatch {						// Struct for our patches of grass on terrain
	float m_rotation[3];
	float m_location[3];
	bool m_active;
	bool m_visible;
	float m_alpha;
} grassPatch;

typedef struct grassQuad {						//struct for a block containing many pieces of grass
	float x1, z1, x2, z2;
	bool visible;
	float m_alpha;
	grassPatch *m_grass;
} grassQuad;

typedef struct mainQuad {						//the main set of blocks extending across the landscape
	grassQuad m_quad[16][16];
} mainQuad;

class SphTerrain
{
	//private variables for Terrain structure
   private:
	
	bool m_hasTexture, m_hasColor, m_hasWater, m_detailMapped, m_rendermode;

	int m_startX, m_startY, m_startZ;				
	int m_length, m_width, m_stepSize;
	int m_textureTile, m_waterHeight, m_detTile;

	float **m_heightData;
	float m_scaleX, m_scaleY, m_scaleZ;
	float m_vertexColorX, m_vertexColorY, m_vertexColorZ;
	float m_average;
	float* m_scale;
	float* m_start;
	int* m_bounds;

	int m_grassCount;
	uint m_grasstexture;
	float m_innerRadius;
	float m_outerRadius;

	float m_collidableX[5];
	float m_collidableZ[5];

	uint m_terrainTexture;
	uint m_terrainDetail;
	uint m_waterTexture;

	BEZIER_PATCH m_bezierCurve;		
	POINT_3D **m_normalList;
	mainQuad m_grassQuad;

	float m_minHeight;
	int m_grassBlocksActive;

	//all of our public variables
   public : 

	SphObject **m_boundaries;

	SphTerrain();
	SphTerrain(char* filename);

	virtual ~SphTerrain();
	virtual void freeHeightData();

	virtual void SphTerrain::loadHeightTexture(char* filePath);
	virtual uint SphTerrain::loadTexture(char* filePath);	
	virtual uint SphTerrain::loadDetailTexture(char* filePath, int tilesize);

	virtual void SphTerrain::setSize(int width, int length);
	virtual void SphTerrain::setStepSize(int stepsize);
	virtual void SphTerrain::setStartingCoordinates(int x, int y, int z);
	virtual void SphTerrain::setHeight(int x, int z, float height);   
	virtual void SphTerrain::setTexture(char* filePath);
	virtual void SphTerrain::setRenderMode(bool lines);
	virtual void SphTerrain::setTiles(int tilecount);
	virtual void SphTerrain::setVertexColor(float x, float y, float z);
	virtual void SphTerrain::setTerrainScale(float m_scaleX, float m_scaleY, float m_scaleZ);
	virtual void SphTerrain::setTextureMode(bool on_off);
	virtual void SphTerrain::setWater(bool active, int wheight);
	virtual void SphTerrain::setBezierActive(int stepsize, int pointsPerPatch);
	virtual void SphTerrain::setwaterTexture(char* filePath);
	virtual void SphTerrain::setDetailTexture(bool bEnabled, char* filePath, int tilesize);
	virtual void SphTerrain::setBoundaries(int x, int z);
	virtual void SphTerrain::setupDetailTextures();
	virtual void SphTerrain::applyMatrix();
	virtual void SphTerrain::renderWithQuadStrips();
	virtual bool SphTerrain::needToBeRendered(float X, float Z, float camx, float camy, float camz);
	virtual void SphTerrain::renderUsingQuads(float camx, float camy, float camz);
	virtual void SphTerrain::renderWithDetailMap();
	virtual void SphTerrain::renderWithoutDetailMap(float camx, float camy, float camz);

	virtual void SphTerrain::CreateFlatPlane(int x, int z, int width, int length, float height);
	virtual void SphTerrain::createFlatPlaneSmoothEdges(int x, int z, int width, int length, int steps, float height);

	//for getting data regarding Heightmap
	virtual int SphTerrain::getWidth() const;
	virtual int SphTerrain::getlength() const;
	virtual int SphTerrain::getStepSize() const;
	virtual float* SphTerrain::getScale() const;
	virtual float* SphTerrain::getStartingCoordinates() const;
	virtual float SphTerrain::getHeight(int x,int z) const;
	virtual float SphTerrain::getHeightc(float X, float Z) const;
	virtual float SphTerrain::getWaterHeight();
	virtual int* SphTerrain::getBoundaries();

	virtual void SphTerrain::renderTerrain(float camx, float camy, float camz, float angle, float fov);

	virtual void SphTerrain::smoothTerrain(int intensity, int block_size, int iterations);
	virtual void SphTerrain::smoothTerrainEdges(int m_StepSize);

	virtual void SphTerrain::initializeBezierCurves(int x, int z, int width, int height, int sidelength);

	virtual void SphTerrain::calculateNormals();
	virtual void SphTerrain::renderWater();
	virtual void SphTerrain::calculateBoundaries();

	virtual void SphTerrain::initializeGrass(float x, float z, float innerRadius, float outerRadius, int quantity, float minHeight);
	virtual void SphTerrain::updateGrass(cfloat* campos, cfloat* pos, cfloat* la);
	virtual void SphTerrain::renderGrass();
	virtual void SphTerrain::loadGrassTexture(char* filePath);	
	virtual void SphTerrain::disableGrassPlane(int x, int z, int width, int length);
  
	
};
