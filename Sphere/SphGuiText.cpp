#include "SphTextRenderer.h"
#include "SphGuiText.h"

//Create SphGuiText.
//length - number of bytes allocated for the text.
SphGuiText::SphGuiText(int length) : m_text(new char[length]){
	this->m_text[0] = '\0';
	this->m_font = SphTextRenderer::s_defaultFont;
}

//Render SphGuiText.
void SphGuiText::Render(){
	g_textRenderer.SetFont(m_font);
	g_textRenderer.Render(m_text, m_left[O_X], m_left[O_Y]);
	SphGuiObject::Render();
}

//Set left coordinate of this text to (x, y - fontHeight).
//return - final y coordinate.
float SphGuiText::AllignToBottom(float x, float y){
	m_left[O_X] = x;
	m_left[O_Y] = y - g_textRenderer.GetFontHeight(m_font);
	return m_left[O_Y];
}

//Destroy SphGuiText and free all resources.
SphGuiText::~SphGuiText(){
	delete [] m_text;
}
