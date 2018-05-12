#pragma once
#include <stdio.h>
#include "SphInclude.h"

//TgaImage is borrowed with some modifications from NeHe (http://nehe.gamedev.net) tutorial #33.
//TgaImage loads a .tga image file.
class TgaImage {

private:
	void LoadCompressedTga(FILE* file);

public:
	uint  m_bytesPerPixel;
	uint  m_height;
	uint  m_width;
	byte* m_data;

	TgaImage(cchar* filename);
	~TgaImage();
};
