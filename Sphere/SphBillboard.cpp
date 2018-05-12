#include "SphBillboard.h"
#include "SphOpenGL.h"
#include "SphString.h"
#include "SphMath.h"

//Create SphObject.
SphBillboard::SphBillboard(){
	
}

//Load SphBillboard's texture from a TGA file.
//directory - containing the file.
//filename - of the billboard texture to load.
//return - true if the texture was loaded successfully.
bool SphBillboard::Load(cchar* directory, cchar* filename){
	char* filePath = ConcatNew(directory, filename);
	
	this->m_texture = g_openGL.LoadTexture(filePath);

	delete filePath;

	return true; //TODO: add error checking for this function
}

//Renders a textured square to the screen
void SphBillboard::RenderTexturedSquare()
{
	glBindTexture( GL_TEXTURE_2D, m_texture );
	glEnable(GL_BLEND);
	glBegin( GL_QUADS );
	{	// a square centered at the origin (so that billboard applications are correct)
		glTexCoord2d(0.0,0.0); glVertex2d(-2.0,-2.0);
		glTexCoord2d(1.0,0.0); glVertex2d(+2.0,-2.0);
		glTexCoord2d(1.0,1.0); glVertex2d(+2.0,+2.0);
		glTexCoord2d(0.0,1.0); glVertex2d(-2.0,+2.0);
	}
	glEnd();
	glDisable(GL_BLEND);
}