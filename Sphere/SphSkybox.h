#pragma once
#include "SphInclude.h"

class SphSkybox
{

	protected:
	 float m_locationX, m_locationY, m_locationZ;				//handles the center point of the skybox, should be set to camera position
	 float m_vertexColorX, m_vertexColorY, m_vertexColorZ;		//maintains vertex color for adding color to skybox texture;
	 float m_distance;											//maintains the apothem from camera to wall
	 int m_textureCount;										//keeps track of the number of textures we have
	 bool m_hasColor;											//lets us know whether we're using color on walls
	 uint m_texture[7];											//stores our 6 .tga textures

	public: 

	 SphSkybox();												//default constructor
	 SphSkybox(float x, float y, float z, float dist);			//basic constructor
     ~SphSkybox();												//free up memory
     virtual void setRange(float distance);						//sets the apothem distance
	 virtual void renderSkybox();								//renders the actual skybox
	 virtual void setVertexColor(float x, float y, float z);	//changes vertex color and sets use color to true
	 virtual void setTextures(char* left, char* right, char* up, char* down, char* front, char* back);	//sets the wall textures
	 virtual uint loadTexture(char* filePath);					//loads a specific texture based on file path
	 virtual void setLocation(float x, float y, float z);		//sets center location of skybox, should be set to camera position

};

