#include <stdio.h>
#include "SphOpenGL.h"
#include "SphMath.h"
#include "SphTextRenderer.h"

SphTextRenderer g_textRenderer;

cfloat l_defaultSpaceSizeU = 0.3f;

cfloat SphTextRenderer::s_charSizeU = 1.0f;
cfloat SphTextRenderer::s_charSizeV = 1.0f / (float)SphTextRenderer::s_charCount;

//Create SphTextRenderer.
SphTextRenderer::SphTextRenderer(){
	this->m_font = &m_fonts[s_defaultFont];
	Set4(this->m_color, g_ones);
	this->m_textScale = 1.0f;
}

//Initialize SphTextRenderer.
//return - 0 on success.
int SphTextRenderer::Init(){
	LoadFont(&m_fonts[s_defaultFont], "Resources/DefaultFont.tga", "Resources/DefaultFont.txt");
	LoadFont(&m_fonts[s_defaultMonoFont], "Resources/DefaultMonoFont.tga");
	return 0;
}

//text - to render to the screen.
//x, y - top left corner of text.
void SphTextRenderer::Render(cchar* text, float x, float y){
	glBindTexture(GL_TEXTURE_2D, m_font->m_texture);
	RenderString(text, x, y);
}

//font - to load info for.
//textureFilePath - file path to the font texture.
//infoFilePath - file path to the font info file. NULL to use default values.
void SphTextRenderer::LoadFont(SphFont* font, cchar* textureFilePath, cchar* infoFilePath){
	int textureSize[N_XY];
	font->m_texture = g_openGL.LoadFontTexture(textureFilePath, textureSize);
	if(infoFilePath == NULL){
		SetN(font->m_charSizeUs, s_charSizeU, s_charCount);
	} else {
		FILE* file = fopen(infoFilePath, "r");
		font->m_charSizeUs[0] = l_defaultSpaceSizeU;
		for(int n = 1; n < s_charCount; n++){
			float value;
			fscanf(file, "%f\n", &value);
			font->m_charSizeUs[n] = value;
		}
		fclose(file);
	}
	font->m_charSizeV = s_charSizeV;
	font->m_charWidth = textureSize[O_X] / 640.0f;
	font->m_charHeight = textureSize[O_Y] / s_charCount / 480.0f;
}

//string - string to render.
//x, y - top position of the string.
void SphTextRenderer::RenderString(cchar* string, float x, float y){
	float startX = x;
	glBegin(GL_QUADS);
	glColor4fv(m_color);

	char glyph;
	while((glyph = *string++) != '\0'){
		if(glyph == '\n'){
			x = startX;
			y += m_textScale * m_font->m_charHeight;
		} else {
			x += RenderGlyph(glyph, x, y);
		}
	}

	glEnd();
}

//glyph - character to render.
//x, y - top position of the glyph.
//return - width of the drawn glyph in pixels.
float SphTextRenderer::RenderGlyph(char glyph, float x, float y){
	int charOffset = glyph - s_firstChar;
	float sMin = 0.0f;
	float sMax = m_font->m_charSizeUs[charOffset];
	float tMax = 1.0f - charOffset * m_font->m_charSizeV;
	float tMin = tMax - m_font->m_charSizeV;
	float xSize = m_textScale * m_font->m_charWidth * m_font->m_charSizeUs[charOffset];
	float ySize = m_textScale * m_font->m_charHeight;

	glTexCoord2f(sMax, tMax); glVertex3f(x + xSize, y,         1.0f);
	glTexCoord2f(sMin, tMax); glVertex3f(x,         y,         1.0f);
	glTexCoord2f(sMin, tMin); glVertex3f(x,         y + ySize, 1.0f);
	glTexCoord2f(sMax, tMin); glVertex3f(x + xSize, y + ySize, 1.0f);
	return xSize;
}

//font - index to use for rendering text.
void SphTextRenderer::SetFont(int font){
	m_font = &m_fonts[font];
}

//font - index.
//return - font's height.
float SphTextRenderer::GetFontHeight(int font){
	return m_fonts[font].m_charHeight;
}

//return - width of the text in texture units.
float SphTextRenderer::GetTextWidth(cchar* text){
	float width = 0.0f;
	char glyph;
	while((glyph = *text++) != '\0'){
		if(glyph != '\n'){
			width += m_textScale * m_font->m_charWidth * m_font->m_charSizeUs[glyph - s_firstChar];
		}
	}
	return width;
}
