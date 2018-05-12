#pragma once
#include "SphInclude.h"

//SphTextRenderer can display text strings.
class SphTextRenderer{

private:
	static cchar s_firstChar = ' ';
	static cint s_charCount = 96;
	static cfloat s_charSizeU;							//default width of a char in texture units
	static cfloat s_charSizeV;							//default height of a char in texture units

	//SphAlphabet stores information about a particular font.
	struct SphFont{
		uint m_texture;									//all characters in the texture are listed vertically and are left alligned
		float m_charWidth;								//width of full length character in pixels
		float m_charHeight;								//height of a character in pixels
		float m_charSizeV;								//height of each character in texture units
		float m_charSizeUs[s_charCount];				//width of each character in texture units
	};

	static cint s_fontCount = 2;
	SphFont m_fonts[s_fontCount];
	SphFont* m_font;
	float m_color[N_RGBA];
	float m_textScale;

	void LoadFont(SphFont* font, cchar* textureFilePath, cchar* infoFilePath = NULL);
	void RenderString(cchar* string, float x, float y);
	float RenderGlyph(char glyph, float x, float y);

public:
	static cint s_defaultFont = 0;
	static cint s_defaultMonoFont = 1;

	SphTextRenderer();
	int   Init();
	void  Render(cchar* text, float x, float y);
	void  SetFont(int font);
	float GetFontHeight(int font);
	float GetTextWidth(cchar* text);
};

extern SphTextRenderer g_textRenderer;
