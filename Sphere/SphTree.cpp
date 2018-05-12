/*
SphTree.cpp, written by Joseph DiGiovanna, jdigiov2
*/
#pragma once
#include "SphOpenGL.h"
#include "SphString.h"
#include "SphMath.h"
#include "SphTgaImage.h"
#include "SphTightBound.h"
#include "SphSphericBound.h"
#include "SphWorld.h"
#include "SphTree.h"
#include <iostream>

/*
Default Constructor for SphTree
*/
SphTree::SphTree()
{
	m_tree = new tBranch;
}

/*
Constructor with required information to place and grow tree
*/
SphTree::SphTree(float locX, float locY, float locZ, float Sx, float Sy, float Sz, float angleX, float angleY, float length, float radius, float lengthSprout, float growspeed)
{
    m_tree = new tBranch;

	m_tree->X = locX;
	m_tree->Y = locY;
	m_tree->Z = locZ;
	
	m_tree->ScaleX = Sx;
	m_tree->ScaleY = Sy;
	m_tree->scaleZ = Sz;

    m_tree->angleX = angleX;
    m_tree->angleY = angleY;
    
    m_tree->length = length;
    m_tree->radius = radius;

    m_tree->lengthSprout = lengthSprout;
	m_tree->growSpeed = growspeed;

    m_tree->left = NULL;
    m_tree->right = NULL;
}

/*
Set information function, does everything advanced constructor does in case
we choose to use the default constructor for anything or want to reset information
*/
void SphTree::setInformation(float locX, float locY, float locZ, float Sx, float Sy, float Sz, float angleX, float angleY, float length, float radius, float lengthSprout, float growspeed, SphWorld* m_world)
{
	m_tree->X = locX;
	m_tree->Y = locY;
	m_tree->Z = locZ;
	
	m_tree->ScaleX = Sx;
	m_tree->ScaleY = Sy;
	m_tree->scaleZ = Sz;

    m_tree->angleX = angleX;
    m_tree->angleY = angleY;
    
    m_tree->length = length;
    m_tree->radius = radius;

    m_tree->lengthSprout = lengthSprout;
	m_tree->growSpeed = growspeed;

    m_tree->left = NULL;
    m_tree->right = NULL;

	float m_pos[3];
	m_pos[0] = locX; m_pos[1] = locY + 0.25f; m_pos[2] = locZ;

	m_pos[1] += 1.0f;
	this->m_boundry.m_bound = new SphSphericBound(&this->m_boundry, 1.5f);
	this->m_boundry.SetPosition(m_pos, true);
	m_world->AddCollidable(&this->m_boundry);
}

/*
New branch function, works when a fork is called in procedural tree creation
*/
tBranch *newBranch(float angleX, float angleY, float length, float radius, float lengthSprout, float growSpeed){
 
    tBranch *branch;

    branch = new tBranch;
    if(!branch)
        return NULL;

    branch->angleX = angleX;
    branch->angleY = angleY;
    
    branch->length = length;
    branch->radius = radius;

    branch->lengthSprout = lengthSprout;
    branch->growSpeed = growSpeed;

    branch->left = NULL;
    branch->right = NULL;

    return branch;

}

/*
Grow branch function uses dt as a function of length and radius
can be done during real time to enlarge and fork a tree.
*/
void SphTree::growBranch(tBranch *branch, float dt){
    float angleXleft, angleYleft;
    float angleXright, angleYright;
    float growLeft, growRight;
    float lengthSproutLeft, lengthSproutRight;
    float radiusLeft, radiusRight;

    if(branch == NULL) return;

    branch->length += branch->growSpeed * dt;
    branch->radius += branch->growSpeed * 0.1f * dt;

    if(branch->length > branch->lengthSprout)
    {
		// if has no children yet, generate 2 new branchs
        if(!branch->left)              
        {
            angleXleft = ((double)rand() / (double)(RAND_MAX)) * 90.0f;
            angleYleft = ((double)rand() / (double)(RAND_MAX)) * 360.0f;
            angleXright = angleXleft - 90;
            angleYright = angleYleft - 180;

            growLeft = cos(angleXleft / 57.2f) * branch->growSpeed;
            growRight = cos(angleXright / 57.2f) * branch->growSpeed;
            
            lengthSproutLeft = ((double)rand() / (double)(RAND_MAX)) * 0.1f;
            lengthSproutRight = ((double)rand() / (double)(RAND_MAX)) * 0.1f;
            
            radiusLeft = growLeft * branch->radius;
            radiusRight = growRight * branch->radius;
            
			if (branch->radius > 0.02){
				branch->left = newBranch(angleXleft, angleYleft, 0.0f, radiusLeft, lengthSproutLeft, growLeft);
				branch->right = newBranch(angleXright, angleYright, 0.0f, radiusRight, lengthSproutRight, growRight);
			}
        }
    }

    growBranch(branch->left, dt);
    growBranch(branch->right, dt);
}

/*
clean up
*/
void SphTree::destroyBranch(tBranch *branch){
    if(branch == NULL)
        return;

    destroyBranch(branch->left);
    destroyBranch(branch->right);

    free(branch);
}
	
/*
Render the branch using a recursive quadric GluCylinder.  Different types of LOD's take place here
based on distance from tree and the radius of the branch.

The commented out section is what does leaves for the tree.
*/
void SphTree::renderBranch(tBranch *branch, GLUquadricObj *quad, int LOD){

	if(branch == NULL)
        return;

    glRotatef(branch->angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(branch->angleY, 0.0f, 1.0f, 0.0f);

	// line up cylinder along the y-axis
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);        
	
	int TLOD = LOD;
	if (branch->radius < 0.04f)
		TLOD = 3;
	if (branch->radius < 0.03f)
		TLOD = 2;
	if (branch->radius < 0.02f)
		TLOD = 1;
	
	glBindTexture(GL_TEXTURE_2D, this->m_treeTexture);
	gluCylinder(m_quad, branch->radius, branch->radius * 0.9f, branch->length *1.05, TLOD, 1);

	//for rendering leaves

	/*if (branch->left == NULL && branch->right == NULL ){
		glBindTexture(GL_TEXTURE_2D, m_leafTexture);
		//glAlphaFunc ( GL_GREATER,(GLclampf) 0.6 ) ;
		//glEnable ( GL_ALPHA_TEST ) ;

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		//glColor4f(1.0f,1.0f,1.0f, 0.9);	

		gluCylinder(quad, branch->radius * 10.0f, branch->radius * 10.0f, branch->length *1.125, 2, 1);
		//gluSphere(quad, branch->radius * 4.0f, 6, 6);

		//glColor4f(1.0f,1.0f,1.0f, 1.0f);	
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		//glDisable ( GL_ALPHA_TEST ) ;
	}*/

    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

	// translate to the end of the branch, so the next ones start in the correct place 
    glTranslatef(0.0f, branch->length , 0.0f);   

	// render child branches
    renderBranch(branch->left, quad, LOD);           
    renderBranch(branch->right, quad, LOD);

    glTranslatef(0.0f, -branch->length, 0.0f);

    glRotatef(-branch->angleY, 0.0f, 1.0f, 0.0f);
    glRotatef(-branch->angleX, 1.0f, 0.0f, 0.0f);

}

/*
initialize all tree data and load the texture in.
*/
void SphTree::init(){
    m_quad = gluNewQuadric();
	gluQuadricNormals(m_quad, GLU_SMOOTH);
	gluQuadricDrawStyle(m_quad, GLU_FILL);
	gluQuadricTexture(m_quad, true);	
	this->m_treeTexture = loadTexture("Textures/Landscape_Textures/treetex.tga");
	glBindTexture(GL_TEXTURE_2D, this->m_treeTexture);
	growBranch(m_tree, 4);
}

/*
Setup the texture for our trees.
*/
uint SphTree::loadTexture(char* filePath){

	TgaImage tgaImage(filePath);

	uint textureID = 0;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLint components = (tgaImage.m_bytesPerPixel == 3) ? GL_RGB : GL_RGBA;
	gluBuild2DMipmaps(GL_TEXTURE_2D, components, tgaImage.m_width, tgaImage.m_height, components, GL_UNSIGNED_BYTE, tgaImage.m_data);

	return textureID;
}

/*
Render the tree, which is what calls render branch.
This uses math to frustum cull different trees.
Also uses a LOD on the tree system
*/
void SphTree::renderTree(cfloat* pos, cfloat* la){
	
	float distance = 0;
	float lai[3];
	float magnitude;
	float tmppos[3];
	float chkdist = 0;
	float finaldist = 0;
	float angle;
	int LOD;

	//calculate our look vector
	magnitude = la[0] * la[0] + la[1] * la[1] + la[2] * la[2];
	magnitude = sqrt(magnitude);
	
	//normalize it
	lai[0] = la[0] / magnitude;
	lai[1] = la[1] / magnitude;
	lai[2] = la[2] / magnitude;

	//shift one point ahead one look value to create triangle
	tmppos[0] = pos[0] + lai[0];
	tmppos[1] = pos[1];
	tmppos[2] = pos[2] + lai[2];
	
	//get first sides length
	chkdist = (this->m_tree->X - tmppos[0]) * (this->m_tree->X - tmppos[0]) + (this->m_tree->Z - tmppos[2]) * (this->m_tree->Z - tmppos[2]);
	chkdist = sqrt(chkdist);
	
	//get our second sides length
	distance = (this->m_tree->X - pos[0]) * (this->m_tree->X - pos[0]) + (this->m_tree->Z - pos[2]) * (this->m_tree->Z - pos[2]);
	distance = sqrt(distance);
	
	//calculate angle from SSS triangle
	angle = acos(((distance * distance) + 1 - (chkdist * chkdist)) / (2 * distance));
	
	//make sure angle is inside view frustum
	if (angle < (3.14159 * 45 / 180) || distance < 18) {
		
		glPushMatrix();
		
		//Position our tree
		glTranslatef(this->m_tree->X, this->m_tree->Y, this->m_tree->Z);
		
		//Scale tree
		glScalef(this->m_tree->ScaleX, this->m_tree->ScaleY, this->m_tree->scaleZ);
		
		glBindTexture(GL_TEXTURE_2D, this->m_treeTexture);
		
		//Set our LOD based on distance
		if (distance < 12){
			LOD = 8;}
		else if (distance < 25){
			LOD = 6;}
		else if (distance < 50){
			LOD = 4;}
		else 
			LOD = 3;

		//Start the recursive rendering function
		this->renderBranch(this->m_tree, this->m_quad, LOD);
		
		glPopMatrix();

	}

	
}