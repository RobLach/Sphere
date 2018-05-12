#pragma once
#include "SphInclude.h"
#include "SphGuiObject.h"

//SphGuiText is a based GUI object, which is displayed as a text string.
class SphGuiText : public SphGuiObject {

public:
	char* const m_text;
	int m_font;

	SphGuiText(int length);
	void Render();
	float AllignToBottom(float x, float y);
	~SphGuiText();
};
