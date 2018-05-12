#pragma once
#include "SphInclude.h"
#include "SphGuiText.h"

//SphGuiTextbox is a basic textbox, allowing the user to input, select, and edit text.
class SphGuiTextbox : public SphGuiText {

protected:
	float m_totalTime;
	int m_charPos;
	bool m_inputFinished;

public:
	bool m_hasFocus;

	SphGuiTextbox(int length);
	void Render();
	void Update(float seconds);
	void Reset();
	bool IsInputFinished();
};
