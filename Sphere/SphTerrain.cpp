/*
SphTerrain.cpp, created by Joseph DiGiovanna, jdigiov2 with the 
intent of making landscapes easy to produce in an OpenGL environment
*/

#pragma once
#include "SphOpenGL.h"
#include "SphString.h"
#include "SphMath.h"
#include "SphSkybox.h"
#include "SphTgaImage.h"
#include "SphTerrain.h"
#include "SphTightBound.h"
#include "SphTree.h"
#include "SphWorld.h"
#include <iostream>

//functions used with rest of class
//***********************************************************************
// Adds 2 points.
POINT_3D pointAdd(POINT_3D p, POINT_3D q) {
	p.x += q.x;		p.y += q.y;		p.z += q.z;
	return p;
}

// Multiplies a point and a constant. 
POINT_3D pointTimes(float c, POINT_3D p) {
	p.x *= c;	p.y *= c;	p.z *= c;
	return p;
}

// Function for quick point creation
POINT_3D makePoint(float a, float b, float c) {
	POINT_3D p;
	p.x = a;	p.y = b;	p.z = c;
	return p;
}

// Calculates 3rd degree polynomial based on array of 4 points
// and a single variable (u) which is generally between 0 and 1
POINT_3D cubicCurve(float u, POINT_3D *p) {
	POINT_3D	a, b, c, d, r;

	a = pointTimes(pow(u,3), p[0]);
	b = pointTimes(3*pow(u,2)*(1-u), p[1]);
	c = pointTimes(3*u*pow((1-u),2), p[2]);
	d = pointTimes(pow((1-u),3), p[3]);

	r = pointAdd(pointAdd(a, b), pointAdd(c, d));

	return r;
}

//***********************************************************************

/*
default constructor
*/
SphTerrain::SphTerrain(){
	m_heightData = NULL;
}

/*
main Constructor
loads terrain based on a filename
and sets default stepsize to 8
also sets color to false and m_detailMapped to false
*/
SphTerrain::SphTerrain(char* strName) {
	m_heightData = NULL;
	this->m_stepSize = 16;
	this->m_detailMapped = false;
	this->m_hasColor = false;
	loadHeightTexture(strName);
}

/*
Free up memory
*/
SphTerrain::~SphTerrain(){
	freeHeightData();                           
}

/*
Frees the m_heightData for cleaning up 
memory used.
*/
void SphTerrain::freeHeightData(){
	if (m_heightData==NULL) // if already freed
		return;
      
	for (int x=0;x < (m_width - 1) ;x++)
	{
	   delete m_heightData[x];   
	}

	this->m_width = 0;
	this->m_length = 0;
	delete m_heightData;
	m_heightData = NULL; 
}

/*
This activates bezier curves on the landscape 
goes from 0 to width - stepsize and then we 
use smooth terrain to get the final stepsize blocks
*/
void SphTerrain::setBezierActive(int stepsize, int pointsPerPatch){
	for (int i = 0; i <= this->m_width - stepsize; i = i + stepsize / 2){
		for (int j = 0; j <= this->m_length - stepsize; j = j + stepsize / 2){
			initializeBezierCurves(i, j, i + stepsize, j + stepsize, stepsize);
		}
	}
}

/*
This function recalculates the height values of a specific
block of verticies starting at x, z and ending at
x+width, z+height.  The side length desides the variable
ssize, which is then used as the step size for the 16 control
points that will be defined on our block.  

It then calculates 4 cubicCurve cubic curves for each set of
4 control points in each direction.  Then it goes ahead and
resets the height values for the verticies in the block 
according to where they would be along the bezier curve.  

It gives the terrain a very nice look and is much smoother and
easier for the sphere to roll over.  It also stops spikes from 
occuring where pixels on the heightmap aren't considered to have
smooth enough a transition.
*/
void SphTerrain::initializeBezierCurves(int x, int z, int width, int height, int sidelength){
	
	int			u = 0, v;						//indicate points along the curve
	float		py, px;							//indicate our percentage along an axis
	GLuint		drawlist = glGenLists(1);		// make the display list

	POINT_3D	temp[4];
	POINT_3D	*last = (POINT_3D*)malloc(sizeof(POINT_3D)*(2048)); // array of points to mark the first line of polys	
												
	int ssize = sidelength / 4;

	for (int i = 0; i < sidelength; i = i + (ssize)){
		for (int j = 0; j < sidelength; j = j + (ssize)){
			m_bezierCurve.anchors[i / (ssize)][j / (ssize)] = makePoint((float)(x + i), m_heightData[x + i][z + j], (float)(z + j));
		}
	}

	m_bezierCurve.dlBPatch = NULL;

	if (m_bezierCurve.dlBPatch != NULL)					// get rid of any old display lists
		glDeleteLists(m_bezierCurve.dlBPatch, 1);

	temp[0] = m_bezierCurve.anchors[0][3];				// the first derived curve (along x axis)
	temp[1] = m_bezierCurve.anchors[1][3];
	temp[2] = m_bezierCurve.anchors[2][3];
	temp[3] = m_bezierCurve.anchors[3][3];

	for (v = 0; v < sidelength; v++) {									// create the first line of points
		px = ((float)v)/((float)(sidelength-1));						// percent along y axis				
		last[v] = cubicCurve(px, temp);									// use the 4 points from the derives 	
		m_heightData[(int)last[v].x][(int)last[v].z] = last[v].y;		// curve to calculate the points along that curve
	}

	for (u = 1; u < sidelength; u++) {
		py	  = ((float)u)/((float)(sidelength-1));			// Percent along Y axis

		temp[0] = cubicCurve(py, m_bezierCurve.anchors[0]);	// Calculate new bezier points
		temp[1] = cubicCurve(py, m_bezierCurve.anchors[1]);
		temp[2] = cubicCurve(py, m_bezierCurve.anchors[2]);
		temp[3] = cubicCurve(py, m_bezierCurve.anchors[3]);

		for (v = 0; v < sidelength; v++) {
			px = ((float)v)/((float)(sidelength - 1));			// Percent along the X axis

			last[v] = cubicCurve(px, temp);			// Generate new point

			m_heightData[(int)last[v].x][(int)last[v].z] = last[v].y;
		}								
	}

}

/*
This loads a height texture,
then it resets all our array sizes based on the
size of the image.  Finally, it feeds all that image
data into m_heightData.
*/
void SphTerrain::loadHeightTexture(char* filePath){
	TgaImage tgaImage(filePath);
	setSize(tgaImage.m_width, tgaImage.m_height);
	if(tgaImage.m_data == NULL) return;
	for (uint j = 0; j < tgaImage.m_height; j++){
		for (uint i = 0; i < tgaImage.m_width; i++){
			uint flipJ = tgaImage.m_height - j - 1;
			m_heightData[i + 2][j + 2] = tgaImage.m_data[(tgaImage.m_height * flipJ + i)*3];
		}
	}
}

/*
Loads a texture forthe landscape
Returns the uint associated with this texture
*/
uint SphTerrain::loadTexture(char* filePath){

	TgaImage tgaImage(filePath);
	if(tgaImage.m_data == NULL) return 0;
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
This function allows the detail texture to be loaded into memory.
It was necessary to separate this since it uses different information
for texture loading than other methods.
*/
uint SphTerrain::loadDetailTexture(char* filePath, int tilesize){
	TgaImage tgaImage(filePath);
	if(tgaImage.m_data == NULL) return 0;
	uint textureID = 0;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); //set texture environment parameters

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	GLint components = (tgaImage.m_bytesPerPixel == 3) ? GL_RGB : GL_RGBA;
	gluBuild2DMipmaps(GL_TEXTURE_2D, components, tgaImage.m_width, tgaImage.m_height, components, GL_UNSIGNED_BYTE, tgaImage.m_data);

	return textureID;
}

/*
lets you switch between rendering it with lines or filled triangles
*/
void SphTerrain::setRenderMode(bool lines){
	this->m_rendermode = lines;
}

/*
lets you set the step size for the terrain (initally set to 8 in case it's not set)
*/
void SphTerrain::setStepSize(int stepsize){
	if (stepsize < 1) {
		stepsize = 1;}
	else if (stepsize > 1000){
		stepsize = 1000;}
	this->m_stepSize = stepsize;
}

/*
Allows you to enable and set the detail texture 
of a terrain.  It also lets you select tile size
which is the number of tiles per size of the square,
so 8 tilesize provides 64 tiles of the detail texture 
on our landscape.
*/
void SphTerrain::setDetailTexture(bool bEnabled, char* filePath, int tilesize){
	this->m_detailMapped = bEnabled;
	this->m_detTile = tilesize;
	m_terrainDetail = loadDetailTexture(filePath, tilesize);
}

/*
Allows you to set terrain vertex color
When you are using color instead of texture
or both, it allows you to specify the color. 
*/
void SphTerrain::setVertexColor(float x, float y, float z){
	this->m_vertexColorX = x;
	this->m_vertexColorY = y;
	this->m_vertexColorZ = z;
	this->m_hasColor = true;
}

/*
Allows you to enable or disable textures
incase you'd rather use a color for a landscape
*/
void SphTerrain::setTextureMode(bool on_off){
	if (on_off){
		this->m_hasTexture = true;
	}else{
		this->m_hasTexture = false;
	}
}
    
/*
Initially sets the memory space for the terrain based
on step size and image size. higher step size creates
a smaller array for normalsize and boundarysize, which
saves a lot of memory

1024X1024 images would require and additional megabyte 
from m_normalList to make it 1025 X 1025.

quadrupling the number of boundaries by cutting the step size
in half would mean going from 128 X 128 X 124 (bpu) to 256 X 256 X 124bpu
which is an increase of 6+ megabytes.  

So using tight boundaries on array size is very important here for efficiency.
*/
void SphTerrain::setSize(int width, int length){
	
	this->m_length = length;
	this->m_width = width;
   
	for (int b = 0; b <= 3; b++){
		m_start = new float[3];
		m_scale = new float[3];
		m_bounds = new int[3];
	}

	m_heightData = new float*[width + 4];
	for (int x = 0; x < width + 4; x++)
	{
	   m_heightData[x]=new float[length + 4];
		for(int y = 0; y < length + 4; y++){
			m_heightData[x][y] = 0;
		}
	}
  
	int normalsize = ((sizeof(float) * width * 2 + 3) / this->m_stepSize);
	m_normalList = new POINT_3D*[normalsize];
	for (int y = 0; y < normalsize; y++)
	{
		m_normalList[y]=new POINT_3D[normalsize];
	}

	int boundarysize = (width / this->m_stepSize) * 2;
	m_boundaries = new SphObject*[boundarysize];
	for (int z = 0; z < boundarysize; z++){
		m_boundaries[z] = new SphObject[boundarysize];
	}
}
     
/*
Lets the user define starting coodinates that are translated to 
in the rendering loop.  Makes it easier to do things like center
a terrain at the origin if you choose to.
*/
void SphTerrain::setStartingCoordinates(int x, int y, int z){
	this->m_startX = x;
	this->m_startY = y;
	this->m_startZ = z;
}

/*
Returns the step size of the terrain
useful for getting array size, etc...
*/
int SphTerrain::getStepSize() const{
	return this->m_stepSize;
}

//returns the width of the terrain
int SphTerrain::getWidth() const{
	return m_width;
}
	
//returns the length of the terrain
int SphTerrain::getlength() const{
	return m_length;
}

//returns a pointer to a float list of your 3 scale values.
float* SphTerrain::getScale() const{
	m_scale[0] = m_scaleX;
	m_scale[1] = m_scaleY;
	m_scale[2] = m_scaleZ;
	return m_scale;
}

float* SphTerrain::getStartingCoordinates() const{
	m_start[0] = (float)m_startX;
	m_start[1] = (float)m_startY;
	m_start[2] = (float)m_startZ;
	return m_start;
}

/*
get the height of the water for possible collision limitations
*/
float SphTerrain::getWaterHeight(){
	return (float)this->m_waterHeight;
}

/*
Lets you get height if you're using integers
much faster than when using floats becuase there
is no interpolation.
*/
float SphTerrain::getHeight(int X, int Z) const{

	int xtest, ztest;
	xtest = (int)(X / this->m_scaleX);
	ztest = (int)(Z / this->m_scaleZ);

	if (xtest < 0) xtest = 0;
	if (ztest < 0) ztest = 0;
	if (xtest >= this->m_width) xtest = this->m_width - 1;
	if (ztest >= this->m_length) ztest = this->m_length - 1;

	return m_heightData[xtest][ztest] * this->m_scaleY;
}

/*
GetHeightc
Allows you to get the height of the terrain at any given point based on x and z float values.
If you go past the edge of the terrain, it calculates in a repeating fashion using fmod,
so it won't error but assumes the terrain is looping.
*/
float SphTerrain::getHeightc(float X, float Z) const{
	float finalHeight, xtest, ztest, xdist, zdist;
	int origx, origz;
	float da, dd; //da and dd hold distance from a, and from d of the float point to the two main verticies

	xtest = (X) / this->m_scaleX - this->m_startX;
	ztest = (Z) / this->m_scaleZ  - this->m_startZ;

	if (xtest > this->m_width || xtest < 0 || ztest < 0 || ztest > this->m_length) {
		return 0;}
	else {
		//get the coordinates for vertex A.
		origx = (int)(xtest - fmod(xtest, (float)this->m_stepSize));  
		origz = (int)(ztest - fmod(ztest, (float)this->m_stepSize));

		da = (xtest - (float)origx) * (xtest - (float)origx) + (ztest - (float)origz) * (ztest - (float)origz);
		dd = (((float)origx + (float)this->m_stepSize) - xtest) * (((float)origx + (float)this->m_stepSize) - xtest)+ (((float)origz + (float)this->m_stepSize) - ztest) * (((float)origz + (float)this->m_stepSize) - ztest);
		
		//use these variables to now store the difference from point A to the float coordinates
		xtest = fmod(xtest, (float)this->m_stepSize);
		ztest = fmod(ztest, (float)this->m_stepSize);
		
		//if we're outside of boundaries, simply eturn the closest value.
		if (origx + this->m_stepSize > this->m_width || origz + this->m_stepSize > this->m_length)
			return m_heightData[this->m_width - this->m_stepSize][this->m_length - this->m_stepSize];

		//check to see which portion of the quad we're on, then use the appropriate 
		//height values to interpolate to the final value
		if (da < dd){
			finalHeight = m_heightData[origx][origz];
			xdist = xtest / this->m_stepSize;
			finalHeight = finalHeight + (m_heightData[origx + this->m_stepSize][origz] - m_heightData[origx][origz]) * xdist;
			zdist = ztest / this->m_stepSize;
			finalHeight = finalHeight + (m_heightData[origx][origz + this->m_stepSize] - m_heightData[origx][origz]) * zdist;
		}else {
			finalHeight = m_heightData[origx + this->m_stepSize][origz + this->m_stepSize];
			xdist = fabs(this->m_stepSize - xtest) / this->m_stepSize;
			finalHeight = finalHeight + (m_heightData[origx][origz + this->m_stepSize] - m_heightData[origx + this->m_stepSize][origz + this->m_stepSize]) * xdist;
			zdist = fabs(this->m_stepSize - ztest) / this->m_stepSize;
			finalHeight = finalHeight + (m_heightData[origx + this->m_stepSize][origz] - m_heightData[origx + this->m_stepSize][origz + this->m_stepSize]) * zdist ;
		}

		//return the final height * the scale
		return finalHeight * this->m_scaleY + this->m_startY;
	}
}

/*
Smooth Terrain edges
This subroutine is necessary because at the extremes of the 
landscape boundaries (the last 8 units) the bezier curves
have troubles calculating.  This simple algorithm takes those last
8 units and smoothes them out, giving it a much better look.
*/
void SphTerrain::smoothTerrainEdges(int m_StepSize){
	for (int i = 0; i <= m_StepSize; i++){
		for (int j = 0; j <= this->m_length - m_StepSize; j++){
			m_heightData[this->m_length - m_StepSize + i][j] = m_heightData[this->m_width - m_StepSize - i][j];
		}
	}
	for (int t = 0; t <= m_StepSize; t++){
		for (int k = 0; k <= this->m_width - m_StepSize; k++){
			m_heightData[k][this->m_width - m_StepSize + t] = m_heightData[k][this->m_width - m_StepSize - t];
		}
	}
}

/*
Smooth Terrrain
Works by looking through the terrain's verticies in blocks of blocksize.
Then it takes iterations^2 points from that block, finds the heightest and lowest 
points and averages them out.  This is good if you have a terrain that simply has
random spikes that for some reason aren't handled by the bezier curve calculations.
*/
void SphTerrain::smoothTerrain(int intensity, int block_size, int iterations){
	//lp = low point and hp = high point
	int lpx, lpy, lpv, hpx, hpy, hpv;
	int avg = 0;

	lpv = 1000000;	//define minimum value
	hpv = 0;		//define maximum value

	//make sure intensity is within range
	if (intensity < 0) intensity = 0;		
	if (intensity > 10) intensity = 10;

	int blocksize = block_size;

	//make sure block size isn't out of range
	if (blocksize < 1) blocksize = 1;
	if (blocksize > this->m_width || blocksize > this->m_length) blocksize = 4;

	//set our maxwidth and maxheight for our for loops
	int maxwidth = (this->m_width - (this->m_width % blocksize));
	int maxheight = (this->m_length - (this->m_length % blocksize));

	//divide the landscape into blocks
	//loop through all blocks and find extremes in each block, averagging them
	for (int p = 1; p < iterations; p++){
		for (int i = 0; i < maxwidth - blocksize; i = i + blocksize){
			for (int j = 0; j < maxheight - blocksize; j = j + blocksize){
				for (int x = i; x < i + blocksize; x++){
					for (int y = j; y < j + blocksize; y++){
						if (m_heightData[x][y] < lpv) {
							lpv = (int)m_heightData[x][y];
							lpx = x;
							lpy = y;
						}
						if (m_heightData[x][y] > hpv) {
							hpv = (int)m_heightData[x][y];
							hpx = x;
							hpy = y;
						}
					}
				}
				
				//set the height of that new coordinate
				//this->setHeight(lpx, lpy, (int)((float)lpv + (float)((this->m_average - (float)lpv) * ((float)intensity * 0.01))));
				//this->setHeight(hpx, hpy, (int)((float)hpv - (float)(((float)hpv - this->m_average) * ((float)intensity * 0.01))));

				//reset our maximum and minimum values
				lpv = 1000000;
				hpv = 0;
			}
		}
	}

}

/*
Lets you set specific point height during run time, for easily altered landscape
*/
void SphTerrain::setHeight(int x, int z, float height){
	if (x >= 0 && x < m_width){
		if (z >= 0 && z < m_width){
			m_heightData[x][z] = height;
		}
	}
}

/*
Lets you set the terrain Scale.  Can be positive or negative 
depending on whether or not you'd like the invert the terrain.
*/
void SphTerrain::setTerrainScale(float m_scaleX, float m_scaleY, float m_scaleZ){
	this->m_scaleX = m_scaleX;
	this->m_scaleY = m_scaleY;
	this->m_scaleZ = m_scaleZ;
}

/*
Lets you set the number of tiles that are in the terrain texture (good for grass / desert / ice where texture is repeating)
*/
void SphTerrain::setTiles(int tilecount){
	this->m_textureTile = tilecount;
}

/*
Lets you set the terrains texture
*/
void SphTerrain::setTexture(char* filePath){
	m_terrainTexture = loadTexture(filePath);
	this->m_hasTexture = true;
}

/*
Lets you set the texture for our water.
*/
void SphTerrain::setwaterTexture(char* filePath){
	m_waterTexture = loadTexture(filePath);
	this->m_hasWater = true;
}

/*
Lets you activate water on the terrain and set it's starting height value
*/
void SphTerrain::setWater(bool active, int wheight){
	this->m_waterHeight = wheight;
	this->m_hasWater = true;
}

/*
Sets up detail texturing during the rendering
*/
void SphTerrain::setupDetailTextures(){
	glActiveTextureARB(GL_TEXTURE0_ARB);									//set our current texture to texture0
	glEnable(GL_TEXTURE_2D);												//enable texturing

	glBindTexture(GL_TEXTURE_2D, m_terrainTexture);							//bind our ground texture

	glActiveTextureARB(GL_TEXTURE1_ARB);									//activate our next texture, texture1
	glEnable(GL_TEXTURE_2D);												//enable texturing again

	glBindTexture(GL_TEXTURE_2D, m_terrainDetail);							//bind our detail texture

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);			//set the texture mode to combine the two

	glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);
}

/*
applies our scale and translation matricies to the landscape
and follows by disabling openGL's internal culling.
*/
void SphTerrain::applyMatrix(){
	glScalef((GLfloat)this->m_scaleX, (GLfloat)this->m_scaleY, (GLfloat)this->m_scaleZ);
	glTranslatef((GLfloat)this->m_startX, (GLfloat)this->m_startY, (GLfloat)this->m_startZ);
}

/*
Uses Quad Strips to render the terrain, more efficient than quads or tri strips
and assures overall conformity of clockwise or counterclockwise rendering scheme.
*/
void SphTerrain::renderWithQuadStrips(){
	float y;
	for (int X = 0; X < (m_width-this->m_stepSize); X += this->m_stepSize){

		glBegin( GL_QUAD_STRIP );
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		//we start by defining 2 points outside the innermost loop
		//then we just continue adding pairs of points inside that loop
		//so that we overlap as little as possible.
		// Get The (X, Y, Z) Value For The Bottom Right Vertex

		y = m_heightData[X + this->m_stepSize][0];
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, ((float)X + this->m_stepSize) / this->m_width, 0);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, ((float)X + this->m_stepSize) * this->m_detTile/ this->m_width, 0);
		glVertex3f((GLfloat)(X + this->m_stepSize), (GLfloat)y, (GLfloat)0);						// Send This Vertex To OpenGL To Be Rendered

		// Get The (X, Y, Z) Value For The Bottom Left Vertex
		y = m_heightData[X][0];
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, ((float)X) / this->m_width, 0);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, ((float)X * this->m_detTile)/ this->m_width, 0);
		glVertex3f((GLfloat)X, (GLfloat)y, (GLfloat)0);	


		for (int Z = this->m_stepSize; Z < (m_width-this->m_stepSize); Z += this->m_stepSize)
		{	

			// Get The (X, Y, Z) Value For The Top Right Vertex
			y = m_heightData[X + this->m_stepSize][Z];

			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, ((float)X + this->m_stepSize)/ this->m_width, 
				((float)Z + this->m_stepSize) / this->m_width);

			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, ((float)X + this->m_stepSize) * this->m_detTile / this->m_width, 
				((float)Z + this->m_stepSize) * this->m_detTile / this->m_width);
			
			glVertex3f((GLfloat)(X + this->m_stepSize), (GLfloat)y, (GLfloat)Z);		// Send This Vertex To OpenGL To Be Rendered

			// Get The (X, Y, Z) Value For The Top Left Vertex
			y = m_heightData[X][Z];

			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, ((float)X)/ this->m_width, 
				((float)Z + this->m_stepSize) / this->m_width);

			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, ((float)X * this->m_detTile)/ this->m_width, 
				((float)Z + this->m_stepSize) * this->m_detTile / this->m_width);

			glVertex3f((GLfloat)X, (GLfloat)y, (GLfloat)Z);	// Send This Vertex To OpenGL To Be Rendered

		}

		glEnd();
	}
}

/*
Using normal calculations to see if a quad is visible to a given camera position
*/
bool SphTerrain::needToBeRendered(float X, float Z, float camx, float camy, float camz){
	
	int ax, az;
	POINT_3D t;
	float TN, TN2;

	ax = (int)((X * this->m_scaleX) + this->m_startX);
	az = (int)((Z * this->m_scaleX) + this->m_startZ);

	t.x = ax - camx;   
	t.y = getHeightc((float)ax, (float)az) - camy; 
	t.z = az - camz; 

	float magnitude = sqrt(t.x * t.x + t.y * t.y + t.z * t.z);
	t.x = t.x / magnitude;
	t.y = t.y / magnitude;
	t.z = t.z / magnitude;

	TN = t.x * m_normalList[(int)(X / this->m_stepSize) * 2][(int)(Z / this->m_stepSize) * 2].x + t.y * m_normalList[(int)(X / this->m_stepSize) * 2][(int)(Z / this->m_stepSize) * 2].y + t.z * m_normalList[(int)(X / this->m_stepSize) * 2][(int)(Z / this->m_stepSize) * 2].z;    // Tx*m_tris[i].nx + Ty*m_tris[i].ny + Tz*m_tris[i].nz;
	TN2 = t.x * m_normalList[(int)(X / this->m_stepSize) * 2 + 1][(int)(Z / this->m_stepSize) * 2 + 1].x + t.y * m_normalList[(int)(X / this->m_stepSize) * 2 + 1][(int)(Z / this->m_stepSize) * 2 + 1].y + t.z * m_normalList[(int)(X / this->m_stepSize) * 2 + 1][(int)(Z / this->m_stepSize) * 2 + 1].z;

	//give it a small amount of leeway incase there is an odd camera viewpoint
	if ((TN < 0.15f) || (TN2 < 0.15f)) 
		return true;
	else
		return false;
}

/*
renders the terrain using GL_QUADS
*/
void SphTerrain::renderUsingQuads(float camx, float camy, float camz){
	float y;
	for (int X = 0; X < (m_width-this->m_stepSize); X += this->m_stepSize){
		for (int Z = 0; Z < (m_width-this->m_stepSize); Z += this->m_stepSize)
		{
			if (needToBeRendered((float)X, (float)Z, camx, camy, camz)){

				// Get The (X, Y, Z) Value For The Bottom Left Vertex
				y = m_heightData[X][Z];
				glTexCoord2f(((float)X)/ this->m_width, ((float)Z) / this->m_width);
				glVertex3f((GLfloat)X, (GLfloat)y, (GLfloat)Z);	

				// Get The (X, Y, Z) Value For The Top Left Vertex
				y = m_heightData[X][Z + this->m_stepSize];
				glTexCoord2f(((float)X) / this->m_width, ((float)Z + this->m_stepSize) / this->m_width);
				glVertex3f((GLfloat)X, (GLfloat)y, (GLfloat)(Z + this->m_stepSize));							// Send This Vertex To OpenGL To Be Rendered

				// Get The (X, Y, Z) Value For The Top Right Vertex
				y = m_heightData[X + this->m_stepSize][Z + this->m_stepSize];
				glTexCoord2f(((float)X + this->m_stepSize) / this->m_width, ((float)Z + this->m_stepSize) / this->m_width);
				glVertex3f((GLfloat)(X + this->m_stepSize), (GLfloat)y, (GLfloat)(Z + this->m_stepSize));							// Send This Vertex To OpenGL To Be Rendered

				// Get The (X, Y, Z) Value For The Bottom Right Vertex
				y = m_heightData[X + this->m_stepSize][Z];
				glTexCoord2f(((float)X + this->m_stepSize) / this->m_width, ((float)Z) / this->m_width);
				glVertex3f((GLfloat)(X + this->m_stepSize), (GLfloat)y, (GLfloat)Z);						// Send This Vertex To OpenGL To Be Rendered
			}
		}
	}
}

/*
Renders with a detail texture
Does not using normal culling, instead 
uses gl Quad Strips
*/
void SphTerrain::renderWithDetailMap(){
	glDisable(GL_LIGHTING);

	glPushMatrix();
	
	//setup detail texture rendering
	setupDetailTextures();

	//apply scale and translation matricies
	applyMatrix();

	//render using efficient quad strips
	renderWithQuadStrips();

	//disable our two detail textures so 
	//they don't mess with other aspects
	//of the game.
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE0_ARB);		
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glEnable(GL_LIGHTING);
}

/*
Allows for terrain rendering without the use of a detail texture
Incorporates normal culling
Uses Gl Quads
*/
void SphTerrain::renderWithoutDetailMap(float camx, float camy, float camz){

	glDisable(GL_LIGHTING);

	glPushMatrix();
	
	//apply translation and scale matricies for given terrain
	applyMatrix();

	//see how we need to texture it based on settings
	if (this->m_hasTexture){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_terrainTexture);
		if (this->m_hasColor){
			glColor3f(this->m_vertexColorX, this->m_vertexColorY, this->m_vertexColorZ);}}
	else {
		if (this->m_hasColor){
		glColor3f(this->m_vertexColorX, this->m_vertexColorY, this->m_vertexColorZ);
		glBindTexture(GL_TEXTURE_2D, NULL);}}

	//see what rendermode we're in
	if(m_rendermode)											
		glBegin( GL_LINES );							
	else 
		glBegin( GL_QUADS );	

	//call rendering from quads given camera position
	renderUsingQuads(camx, camy, camz);
	
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);

}

/*
Render the terrain
Lets you render a terrain based on the cameras position.  
This allows for accurate normal culling, speeding up the terrain
*/
void SphTerrain::renderTerrain(float camx, float camy, float camz, float angle, float fov)
{
	if (this->m_detailMapped){
		renderWithDetailMap();
	}
	else{
		renderWithoutDetailMap(camx, camy, camz);
	}
}

/*
render our water
Does so by creating 10 sheets, and intervaling them upwards with lower alpha value 
this gives the water and interesting looking effect which is easily altered.
*/
void SphTerrain::renderWater(){
	if (this->m_hasWater){
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glBindTexture(GL_TEXTURE_2D, m_terrainDetail);

		glPushMatrix();
		applyMatrix();

		for (int j = 1; j < 10; j++){
			glColor4f(1.0f,1.0f,1.0f,(float)(1/((float)j + 5.0f)));			// Full Brightness, incrememnt the alpha based on height
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);								// Blending Function For Translucency Based On Source Alpha Value

			glBegin( GL_QUADS );
			
				//(1 + (float)j / 50) lets us increment the y value slightly to make it so you can't define edges around other triangles.
				glTexCoord2f((GLfloat)0.0f, (GLfloat)0.0f); glVertex3f((GLfloat)0, (GLfloat)(this->m_waterHeight * (float)(1 + (float)j / 50)),  (GLfloat)0);	// Bottom Left Of The Texture and Quad
				glTexCoord2f((GLfloat)1.0f, (GLfloat)0.0f); glVertex3f((GLfloat)0, (GLfloat)(this->m_waterHeight * (float)(1 + (float)j / 50)), (GLfloat)(this->m_length));	// Bottom Right Of The Texture and Quad
				glTexCoord2f((GLfloat)1.0f, (GLfloat)1.0f); glVertex3f((GLfloat)this->m_width,  (GLfloat)(this->m_waterHeight * (float)(1 + (float)j / 50)),  (GLfloat)(this->m_length));	// Top Right Of The Texture and Quad
				glTexCoord2f((GLfloat)0.0f, (GLfloat)1.0f); glVertex3f((GLfloat)this->m_width,  (GLfloat)(this->m_waterHeight * (float)(1 + (float)j / 50)),  (GLfloat)0);	// Top Left Of The Texture and Quad
				
			glEnd();

		}

		glPopMatrix();
		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);
	}
}

/*
*	Sub calculateTriangleNormals()
*	Runs through all faces calculating both triangle verticies.  During render, it combines them for more efficient culling
*	This combination doesn't cause problems because it's given a 0.15f weight on the dot product check.
*/
void SphTerrain::calculateNormals(){
	POINT_3D normal, a, b, c, v1, v2;
	float magnitude;

	for (int x = 0; x < (this->m_width - this->m_stepSize); x = x + this->m_stepSize){
		for (int z = 0; z < (this->m_length - this->m_stepSize); z = z + this->m_stepSize){

			//get our 3 vertices
			a.x = x * this->m_scaleX; a.z = z * this->m_scaleZ; a.y = m_heightData[x][z] * this->m_scaleY;
			b.x = (x + (this->m_stepSize)) * this->m_scaleX; b.z = z * this->m_scaleZ; b.y = m_heightData[x + this->m_stepSize][z] * this->m_scaleY;
			c.x = x * this->m_scaleX; c.z = (z + (this->m_stepSize)) * this->m_scaleZ; c.y = m_heightData[x][z + this->m_stepSize] * this->m_scaleY;

			//create two vectors from them
			v1.x = b.x - a.x;
			v1.y = b.y - a.y;
			v1.z = b.z - a.z;

			v2.x = c.x - a.x;
			v2.y = c.y - a.y;
			v2.z = c.z - a.z;
	
			//calculate normal vector from cross product
			normal.x = v1.y * v2.z - v1.z * v2.y;
			normal.y = v1.z * v2.x - v1.x * v2.z;
			normal.z = v1.x * v2.y - v1.y * v2.x;

			//calculate magnitude for normalizing
			magnitude = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

			//normalize normal vector
			normal.x = normal.x / magnitude;
			normal.y = normal.y / magnitude;
			normal.z = normal.z / magnitude;

			//make sure it's facing up since landscape has all upward facing triangles
			if (normal.y < 0){
				normal.x = -normal.x;
				normal.y = -normal.y;
				normal.z = -normal.z;
			}

			//feed it into our normal list as the first part of one quad
			m_normalList[(int)(x / this->m_stepSize) * 2][(int)(z / this->m_stepSize) * 2].x = normal.x;
			m_normalList[(int)(x / this->m_stepSize) * 2][(int)(z / this->m_stepSize) * 2].y = normal.y;
			m_normalList[(int)(x / this->m_stepSize) * 2][(int)(z / this->m_stepSize) * 2].z = normal.z;

			//get other 3 verticies
			a.x = (x + (this->m_stepSize)) * this->m_scaleX; a.z = z * this->m_scaleZ; a.y = m_heightData[x + this->m_stepSize][z] * this->m_scaleZ;
			b.x = (x + (this->m_stepSize)) * this->m_scaleX; b.z = (z + (this->m_stepSize)) * this->m_scaleZ; b.y = m_heightData[x + this->m_stepSize][z + this->m_stepSize] * this->m_scaleY;
			c.x = x * this->m_scaleX; c.z = (z + (this->m_stepSize)) * this->m_scaleZ; c.y = m_heightData[x][z + this->m_stepSize] * this->m_scaleY;

			//calculate vectors from these veticies
			v1.x = b.x - a.x;
			v1.y = b.y - a.y;
			v1.z = b.z - a.z;

			v2.x = c.x - a.x;
			v2.y = c.y - a.y;
			v2.z = c.z - a.z;
	
			//calculate normal from cross product
			normal.x = v1.y * v2.z - v1.z * v2.y;
			normal.y = v1.z * v2.x - v1.x * v2.z;
			normal.z = v1.x * v2.y - v1.y * v2.x;

			//calculate magnitude
			magnitude = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

			//normalize normal vector
			normal.x = normal.x / magnitude;
			normal.y = normal.y / magnitude;
			normal.z = normal.z / magnitude;

			//invert if necessary
			if (normal.y < 0){
				normal.x = -normal.x;
				normal.y = -normal.y;
				normal.z = -normal.z;
			}

			//attach to normal list as second part of quad.
			m_normalList[(int)(x / this->m_stepSize) * 2 + 1][(int)(z / this->m_stepSize) * 2 + 1].x = normal.x;
			m_normalList[(int)(x / this->m_stepSize) * 2 + 1][(int)(z / this->m_stepSize) * 2 + 1].y = normal.y;
			m_normalList[(int)(x / this->m_stepSize) * 2 + 1][(int)(z / this->m_stepSize) * 2 + 1].z = normal.z;
		}	
	}

}

/*
Calculate the boundaries for collision detection with sphere object
Keeps a list of (landscape->length / m_stepSize)^2 boundary tight bound objects.
when doing collision, we only later on need to check the one underneath or perhaps surrounding, depends on accuracy needs.
*/
void SphTerrain::calculateBoundaries(){
	for (int i = 0; i <= (this->m_width - this->m_stepSize); i = i + this->m_stepSize){
		for (int j = 0; j <= (this->m_length - this->m_stepSize); j = j + this->m_stepSize){
			this->m_boundaries[(int)(i / this->m_stepSize)][(int)(j / this->m_stepSize)].m_bound = new SphTightBound(&this->m_boundaries[(int)(i/this->m_stepSize)][(int)(j/this->m_stepSize)]);
			((SphTightBound*)m_boundaries[(int)(i / this->m_stepSize)][(int)(j / this->m_stepSize)].m_bound)->loadChunk(

				(i + this->m_startX) * this->m_scaleX, 
				(i + this->m_stepSize + this->m_startX) * this->m_scaleX, 
				(j + this->m_startZ ) *   this->m_scaleZ, 
				(j + this->m_stepSize + this->m_startZ) * this->m_scaleZ, 

				m_heightData[i][j] * this->m_scaleY + this->m_startY,
				m_heightData[i][j+this->m_stepSize] * this->m_scaleY + this->m_startY, 
				m_heightData[i+this->m_stepSize][j+this->m_stepSize] * this->m_scaleY + this->m_startY, 
				m_heightData[i+this->m_stepSize][j] * this->m_scaleY + this->m_startY);
		}
	}
}

/*
This code goes ahead and updates the list of collidables 
that we are keeping track of in order to have the avatar collide
with them properly.
*/
void SphTerrain::setBoundaries(int x, int z){
	this->m_collidableX[0] = (float)x;
	this->m_collidableX[1] = (float)(x - 1);
	this->m_collidableX[2] = (float)(x - 1);
	this->m_collidableX[3] = (float)(x + 1);
	this->m_collidableX[4] = (float)(x + 1);

	this->m_collidableZ[0] = (float)z;
	this->m_collidableZ[1] = (float)(z - 1);
	this->m_collidableZ[2] = (float)(z + 1);
	this->m_collidableZ[3] = (float)(z - 1);
	this->m_collidableZ[4] = (float)(z + 1);
}

/*
This function retrieves the current boundaries
for comparison to see if we need to update them.
If there sphere hasn't moved past the current block 
that it had been on, there is no reason to update.
*/
int* SphTerrain::getBoundaries(){
	m_bounds[0] = (int)this->m_collidableX[0];
	m_bounds[1] = (int)this->m_collidableZ[0];
	return m_bounds;
}

/*
Creates a basic flat plane somewhere on the landscape
This allows for easy placement of puzzles or flat areas
for better bouncing.
*/
void SphTerrain::CreateFlatPlane(int x, int z, int width, int length, float height){
	for (int i = x; i < x + width; i++){
		for (int j = z; j < z + length; j++){
			m_heightData[i][j] = height;
		}
	}
}

/*
Creates a similar flat plane but smoothes out the edges to
remove the pixelated look that occurs when the y values vary too greately.
*/
void SphTerrain::createFlatPlaneSmoothEdges(int x, int z, int width, int length, int steps, float height){
	for (int i = x; i < x + width; i++){
		for (int j = z; j < z + length; j++){
			m_heightData[i][j] = height;
			/*if ((i - x) < steps){
				m_heightData[i][j] += m_heightData[x - 1][j] * (float)((float)(j-z)/(float)(10000/steps));
			}*/
		}
	}
}

/*
Initializes all the grass quads on the landscape
It uses the grasspatch class to maintain all relevant data
*/
void SphTerrain::initializeGrass(float x, float z, float innerRadius, float outerRadius, int quantity, float minHeight){
	float g_startX, g_startZ;

	m_minHeight = minHeight;
	m_grassCount = quantity;
	this->m_innerRadius = innerRadius;
	this->m_outerRadius = outerRadius;

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			
			this->m_grassQuad.m_quad[i][j].m_grass = new grassPatch[m_grassCount + 1];
			
			g_startX = (float)this->m_startX + (((float)i / 16) * (float)this->m_width);
			g_startZ = (float)this->m_startZ + (((float)j / 16) * (float)this->m_length);
			
			this->m_grassQuad.m_quad[i][j].visible = true;
			this->m_grassQuad.m_quad[i][j].x1 = g_startX * this->m_scaleX;
			this->m_grassQuad.m_quad[i][j].z1 = g_startZ * this->m_scaleZ;
			this->m_grassQuad.m_quad[i][j].x2 = (g_startX + (float)this->m_width / 16) * this->m_scaleX;
			this->m_grassQuad.m_quad[i][j].z2 = (g_startZ + (float)this->m_length / 16) * this->m_scaleZ;

			for (int k = 0; k < this->m_grassCount; k++){
				this->m_grassQuad.m_quad[i][j].m_grass[k].m_active = true;
				this->m_grassQuad.m_quad[i][j].m_grass[k].m_visible = true;
				this->m_grassQuad.m_quad[i][j].m_grass[k].m_alpha = 1;
				this->m_grassQuad.m_quad[i][j].m_grass[k].m_location[0] = this->m_grassQuad.m_quad[i][j].x1 + (rand() / (RAND_MAX + 1.0f)) * (this->m_grassQuad.m_quad[i][j].x2 - this->m_grassQuad.m_quad[i][j].x1);
				this->m_grassQuad.m_quad[i][j].m_grass[k].m_location[2] = this->m_grassQuad.m_quad[i][j].z1 + (rand() / (RAND_MAX + 1.0f)) * (this->m_grassQuad.m_quad[i][j].z2 - this->m_grassQuad.m_quad[i][j].z1);
				this->m_grassQuad.m_quad[i][j].m_grass[k].m_location[1] = (float)this->getHeightc(this->m_grassQuad.m_quad[i][j].m_grass[k].m_location[0], this->m_grassQuad.m_quad[i][j].m_grass[k].m_location[2]) + 0.7f;
				if (this->m_grassQuad.m_quad[i][j].m_grass[k].m_location[0] > 200 || this->m_grassQuad.m_quad[i][j].m_grass[k].m_location[2] > 200){
					this->m_grassQuad.m_quad[i][j].m_grass[k].m_location[0] = 200;
					this->m_grassQuad.m_quad[i][j].m_grass[k].m_location[2] = 200;
				}
				this->m_grassQuad.m_quad[i][j].m_grass[k].m_rotation[0] = 0;
				this->m_grassQuad.m_quad[i][j].m_grass[k].m_rotation[1] = 0;
				this->m_grassQuad.m_quad[i][j].m_grass[k].m_rotation[2] = 0;
			}
		}
	}
}

/*
Load our grass texture in
*/
void SphTerrain::loadGrassTexture(char* filePath){
	TgaImage tgaImage(filePath);
	if(tgaImage.m_data == NULL) this->m_grasstexture = 0;
	uint textureID = 0;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLint components = (tgaImage.m_bytesPerPixel == 3) ? GL_RGB : GL_RGBA;
	gluBuild2DMipmaps(GL_TEXTURE_2D, components, tgaImage.m_width, tgaImage.m_height, components, GL_UNSIGNED_BYTE, tgaImage.m_data);

	this->m_grasstexture = textureID;
}

/*
This function uses SSS triangle math to calculate whether or not
a specific block is within the viewing frustum.  If it is, it is rendered
Otherwise, we make sure its not right underneath us, and then cull it.
This subroutine sets up the system so that the renderer has an easy time.
*/
void SphTerrain::updateGrass(cfloat* campos, cfloat* pos, cfloat* la){

	float distance = 0;
	float lai[3];
	float camla[3];
	float magnitude;
	float tmppos[3];
	float tpos[3];
	float cmppos[3];
	float chkdist = 0;
	float angle;

	//Calculate the normalized camera look at vector
	camla[0] = pos[0] - campos[0]; camla[1] = pos[1] - campos[1]; camla[2] = pos[2] - campos[2];
	magnitude = camla[0] * camla[0] + camla[1] * camla[1] + camla[2] * camla[2];
	magnitude = sqrt(magnitude);
	camla[0] = camla[0] / magnitude;
	camla[1] = camla[1] / magnitude;
	camla[2] = camla[2] / magnitude;

	//Create our first position behind the actual camera
	tmppos[0] = campos[0] - camla[0];
	tmppos[1] = campos[1];
	tmppos[2] = campos[2] - camla[2];

	//Use the camera itself as the second position
	tpos[0] = campos[0];
	tpos[1] = campos[1];
	tpos[2] = campos[2];

	//Loop through all our blocks
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {

			//Initially set quad visibility to false
			this->m_grassQuad.m_quad[i][j].visible = false;	

			//Get the point at the center of the quad so which direction you're moving in
			//doesn't affect anything
			cmppos[0] = this->m_grassQuad.m_quad[i][j].x1 + (this->m_grassQuad.m_quad[i][j].x2 - this->m_grassQuad.m_quad[i][j].x1) / 2;
			cmppos[2] = this->m_grassQuad.m_quad[i][j].z1 + (this->m_grassQuad.m_quad[i][j].z2 - this->m_grassQuad.m_quad[i][j].z1) / 2;

			//Now calculate the first positions distance from quad
			distance = (cmppos[0] - tpos[0]) * (cmppos[0] - tpos[0]) + (cmppos[2] - tpos[2]) * (cmppos[2] - tpos[2]);
			distance = sqrt(distance);
			
			//Decide using outer radius whether or not we want to move on rendering
			if (distance < this->m_outerRadius){
				
				//get the second positions distance
				chkdist = (cmppos[0] - tmppos[0]) * (cmppos[0] - tmppos[0]) + (cmppos[2] - tmppos[2]) * (cmppos[2] - tmppos[2]);
				chkdist = sqrt(chkdist);
				
				//Use SSS theorm to solve for the angle
				angle = acos(((chkdist * chkdist) + 1 - (distance * distance)) / (2 * chkdist));

				//If that angle is less than the view frustum, set it to visible
				if (angle < (3.14159 * 55/180) || distance < 20){
					if (distance < this->m_outerRadius){
						this->m_grassQuad.m_quad[i][j].visible = true;	
						this->m_grassQuad.m_quad[i][j].m_alpha = 0.5f + 0.05f * (float)(this->m_outerRadius - distance);
					} else if (distance < this->m_innerRadius){
						this->m_grassQuad.m_quad[i][j].visible = true;	
						this->m_grassQuad.m_quad[i][j].m_alpha = 1;
					}
				}
			}
		}
	}
}

/*
This sub routine actually renders all of our grass.  
Since everything is already sorted, the rendering algorithm 
is nice and simple.  The only thing it needs to check is individual
grass pieces against a minimum rendering height value.
*/
void SphTerrain::renderGrass(){

	//important so that it doesn't mess up alpha blending
	glDisable(GL_LIGHTING);
	
	glEnable(GL_TEXTURE_2D);

	//Make sure we don't cull since these need to be visible from both sides
	glDisable(GL_CULL_FACE);

	glBindTexture(GL_TEXTURE_2D, this->m_grasstexture);

	float cz = -0.0f,cx = 1.0f;
	float r = 1.0f; 

	//set our alpha function to .6 to remove the edges from the texture
	glAlphaFunc ( GL_GREATER,(GLclampf) 0.6 ) ;
    glEnable ( GL_ALPHA_TEST ) ;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {

			//make sure update decided this quad is visible
			if (this->m_grassQuad.m_quad[i][j].visible == true){
				for (int k = 0; k < this->m_grassCount; k++){
					if (m_grassQuad.m_quad[i][j].m_grass[k].m_visible == true){

						//make sure its above minimum height
						if (m_grassQuad.m_quad[i][j].m_grass[k].m_location[1] > m_minHeight){
							
							//Get a new matrix to work with
							glPushMatrix();

							//Position and scale our piece of grass
							glTranslatef((GLfloat)this->m_grassQuad.m_quad[i][j].m_grass[k].m_location[0], (GLfloat)this->m_grassQuad.m_quad[i][j].m_grass[k].m_location[1], (GLfloat)this->m_grassQuad.m_quad[i][j].m_grass[k].m_location[2]);
							glScalef(1.0f, 0.75f, 1.0f);
							glColor4f(1.0f,1.0f,1.0f, this->m_grassQuad.m_quad[i][j].m_alpha);	

							//Render two grass quads in different direction so the grass
							//doesn't dissapear when you rotate the camera in 
							//a specific direction
							glBegin(GL_QUADS);	
								glTexCoord2f(cz, cz); glVertex3f(-1.0f, -1.0f, 1.0f);	
								glTexCoord2f(cx, cz); glVertex3f(1.0f, -1.0f,  1.0f); 
								glTexCoord2f(cx, cx); glVertex3f(1.0f,  1.0f,  1.0f);
								glTexCoord2f(cz, cx); glVertex3f(-1.0f,  1.0f, 1.0f);

								glTexCoord2f(cz, cz); glVertex3f(1.0f, -1.0f, -1.0f);	
								glTexCoord2f(cx, cz); glVertex3f(1.0f, -1.0f,  1.0f); 
								glTexCoord2f(cx, cx); glVertex3f(1.0f,  1.0f,  1.0f);
								glTexCoord2f(cz, cx); glVertex3f(1.0f,  1.0f, -1.0f);
							glEnd();
							
							glPopMatrix();
						}
					}
				}
			}

		}
	}

	glDisable(GL_BLEND);

	glDisable ( GL_ALPHA_TEST ) ;

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);
	
}

/*
Disables a given grass plane, any piece of grass within these boundaries is not rendered.
*/
void SphTerrain::disableGrassPlane(int x, int z, int width, int length){
	float dx, dz, dwidth, dlength;
	dx = (x - this->m_startX) * this->m_scaleX;
	dz = (z - this->m_startZ) * this->m_scaleZ;
	dwidth = width * this->m_scaleX;
	dlength = length * this->m_scaleZ;
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			for (int k = 0; k < this->m_grassCount; k++){
				if (this->m_grassQuad.m_quad[i][j].m_grass[k].m_location[0] > dx && this->m_grassQuad.m_quad[i][j].m_grass[k].m_location[0] < dx + dwidth && this->m_grassQuad.m_quad[i][j].m_grass[k].m_location[2] > dz && this->m_grassQuad.m_quad[i][j].m_grass[k].m_location[0] < dz + dlength){
					this->m_grassQuad.m_quad[i][j].m_grass[k].m_visible = false;
				}
			}
		}
	}
}