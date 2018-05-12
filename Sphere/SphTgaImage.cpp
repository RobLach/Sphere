#include "SphTgaImage.h"
#include "memory.h"

cbyte l_uTgaCompare[12] = {0,0,2, 0,0,0,0,0,0,0,0,0};// Uncompressed TGA Header
cbyte l_cTgaCompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};// Compressed TGA Header

//Create a new TgaImage from the given location.
TgaImage::TgaImage(cchar* filePath){
	FILE* file = fopen(filePath, "rb");
	if(file == NULL){
		m_data = NULL;
		return;
	}

	byte header[12], subHeader[6];
	fread(&header, sizeof(header), 1, file);
	fread(&subHeader, sizeof(subHeader), 1, file);

	m_width   = (subHeader[1] << 8) | subHeader[0];
	m_height  = (subHeader[3] << 8) | subHeader[2];
	int bpp = subHeader[4];

	m_bytesPerPixel = bpp / 8;
	int imageSize = m_bytesPerPixel * m_width * m_height;
	m_data = new byte[imageSize];

	if(memcmp(l_uTgaCompare, header, sizeof(header)) == 0){
		//load raw data and swap red and blue bytes
		fread(m_data, 1, imageSize, file);
		for(int n = 0; n < imageSize; n += m_bytesPerPixel){
			byte temp = m_data[n];
			m_data[n] = m_data[n + 2];
			m_data[n + 2] = temp;
		}
	} else if(memcmp(l_cTgaCompare, header, sizeof(header)) == 0){
		//load the compressed data
		LoadCompressedTga(file);
	}

	fclose(file);
}

//Load data from an compressed TGA file and store it into tgaData.
void TgaImage::LoadCompressedTga(FILE* file){
	uint pixelcount	= m_height * m_width;
	uint currentpixel	= 0;
	uint currentbyte	= 0;
	byte* colorbuffer = new byte[m_bytesPerPixel];

	// Loop while there are still pixels left
	do {
		byte chunkheader = 0;
		fread(&chunkheader, sizeof(byte), 1, file);

		bool rawData = chunkheader < 128;
		if(rawData){
			chunkheader++;
		} else {
			chunkheader -= 127;
			//this is the value we are going to be copying the next X times
			fread(colorbuffer, 1, m_bytesPerPixel, file);
		}

		for(short counter = 0; counter < chunkheader; counter++){
			if(rawData){
				//read in new pixel data
				fread(colorbuffer, 1, m_bytesPerPixel, file);
			}

			//swap red and blue bytes
			m_data[currentbyte + 0] = colorbuffer[2];
			m_data[currentbyte + 1] = colorbuffer[1];
			m_data[currentbyte + 2] = colorbuffer[0];

			if(m_bytesPerPixel == 4){
				m_data[currentbyte + 3] = colorbuffer[3];
			}

			currentbyte += m_bytesPerPixel;
			currentpixel++;
		}
	} while(currentpixel < pixelcount);
}

//Free TGAImage and all the data.
TgaImage::~TgaImage(){
	delete [] m_data;
}
