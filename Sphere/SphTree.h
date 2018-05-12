/*
SphTree.h, created by Joseph DiGiovanna, jdigiov2
*/
#pragma once
#include "SphInclude.h"
#include "SphObject.h"
#include "SphOpenGL.h"
#include "SphWorld.h"

/*
struct for our recursive branch
*/
typedef struct sBranch
{

	float X, Y, Z;
	float ScaleX, ScaleY, scaleZ;
    float angleX;   // difference in angle between child and parent branch
    float angleY;   // rotates the previous angle again, between 0 and 2*pi

    float length;   // the current branch length
    float radius;   // the current branch radius

    float lengthSprout; // The minimum required sprout length
    float growSpeed;    // grow speed / factor for tree
    struct sBranch *left, *right;  // pointers to children branches

	bool hasLeaves;	//whether or not to render leaves at the extemeties

} tBranch;

/*
Our SphTree class everything will be called from
*/
class SphTree {

public: 

	SphObject m_boundry;
    tBranch *m_tree;
    float m_angle;
    GLUquadricObj *m_quad;

	uint m_treeTexture;
	uint m_leafTexture;

	SphTree();
	SphTree(float locX, float locY, float locZ, float Sx, float Sy, float Sz, float angleX, float angleY, float length, float radius, float lengthSprout, float growspeed);
	void SphTree::setInformation(float locX, float locY, float locZ, float Sx, float Sy, float Sz, float angleX, float angleY, float length, float radius, float lengthSprout, float growspeed, SphWorld* m_world);
	void SphTree::growBranch(tBranch *branch, float dt);
	void SphTree::destroyBranch(tBranch *branch);
	void SphTree::renderBranch(tBranch *branch, GLUquadricObj *quad, int LOD);
	void SphTree::init();
	uint SphTree::loadTexture(char* filePath);
	void SphTree::renderTree(cfloat* pos, cfloat* la);
};