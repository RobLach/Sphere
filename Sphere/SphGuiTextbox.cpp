#include "SphSdlInput.h"
#include "SphRenderer.h"
#include "SphTextRenderer.h"
#include "SphGuiTextbox.h"

cfloat l_blinksPerSec = 3.0f;
cfloat l_nudgeFactor = 0.1f;//controls how far the caret edge points are from text and textbox borders.
cfloat l_caretLineWidth = 1.0f;

//Create SphGuiTextbox.
SphGuiTextbox::SphGuiTextbox(int length) : SphGuiText(length){
	this->m_totalTime = 0.0f;
	this->m_drawFrame = false;
	this->Reset();
}

//Render SphGuiTextbox.
void SphGuiTextbox::Render(){
	SphGuiText::Render();
	if(m_hasFocus && (int)(m_totalTime * l_blinksPerSec) % 2 == 0){
		float nudge = l_nudgeFactor * (m_right[O_Y] - m_left[O_Y]);
		float xPos = g_textRenderer.GetTextWidth(m_text);
		RenderGuiLine(m_frameColor, xPos + nudge, m_left[O_Y] + nudge, xPos + nudge, m_right[O_Y] - nudge, l_caretLineWidth);
	}
}

//Update SphGuiTextbox.
//seconds - since last call.
void SphGuiTextbox::Update(float seconds){
	if(!m_hasFocus || m_inputFinished) return;
	m_totalTime += seconds;

	char key = g_sdlInput.GetPrintableKeyDownEvent();
	if(key != '\0'){
		m_text[m_charPos++] = key;
		m_text[m_charPos] = '\0';
	}
	if(g_sdlInput.KeyDownEvent(SDLK_BACKSPACE) && m_charPos > 0){
		m_text[--m_charPos] = '\0';
	}
	if(g_sdlInput.KeyDownEvent(SDLK_RETURN)){
		m_inputFinished = true;
	}

	SphGuiText::Update(seconds);
}

//Set the text to empty, lose the focus.
void SphGuiTextbox::Reset(){
	m_text[0] = '\0';
	m_charPos = 0;
	m_inputFinished = false;
	m_hasFocus = false;
	m_drawFrame = false;
}

//return - false if the user is still typing, true if the user is done typing and hit Enter.
bool SphGuiTextbox::IsInputFinished(){
	return m_inputFinished;
}
