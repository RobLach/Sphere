#include "SphRenderer.h"
#include "SphGuiImage.h"
#include "SphOpenGL.h"
#include "SphString.h"

//Create SphGuiImage.
SphGuiImage::SphGuiImage(){
	this->m_texture = 0;
}

//Render SphGuiImage.
void SphGuiImage::Render(){
	if(m_texture > 0){
		RenderGuiImage(m_texture, m_left, m_right);
		SphGuiObject::Render();
	}
}

//Load SphGuiImage from file
bool SphGuiImage::Load(cchar* directory, cchar* filename){
	char* filePath = ConcatNew(directory, filename);
	FILE* file = fopen(filePath, "r");
	bool success = false;

	if(file){
		this->m_texture = g_openGL.LoadTexture(filePath);

		fclose(file);
		success = true;
	}

	delete filePath;
	return success;
}
