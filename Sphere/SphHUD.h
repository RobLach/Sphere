#pragma once
#include "SphInclude.h"
#include "SphGuiImage.h"
#include "SphGuiObject.h"
#include "SphGuiText.h"
#include "SphGuiTextbox.h"
#include "SphAvatar.h"

//SphHUD is the heads up display and stores all necessary GUI elements.
class SphHUD{

protected:
	static cint s_cfaButtonCount = 4;
	static cint s_ofaButtonCount = 3;
	static cint s_keyButtonCount = 6;
	SphGuiText* m_health;
	SphGuiTextbox* m_console;
	SphGuiImage* m_cfaButtons[s_cfaButtonCount];
	SphGuiImage* m_ofaButtons[s_ofaButtonCount];
	SphGuiImage* m_keyButtons[s_keyButtonCount];
	SphGuiImage* m_activeCFA;
	SphGuiImage* m_mainmenuButtons[3];
	SphGuiImage* m_cacti[7];
	SphGuiImage* m_achievements[2];
	bool m_mainmenuActive;
	SphGuiText* m_cfaTooltip[s_cfaButtonCount];
	SphGuiText* m_ofaTooltip[s_ofaButtonCount];
	SphGuiText* m_keyTooltip;
	bool m_tooltipActive;

private:
	SphAvatar* m_avatar;
	float yscale;
	void UpdateHealth();
	void ConsoleOn();
	void ConsoleOff();
	void MainMenuOn();
	void MainMenuOff();
	void ParseConsoleInput();
	void InitializeCFAButtons();
	void InitializeOFAButtons();
	void InitializeKeyButtons();
	void InitializeTooltips();
	void InitializeCactusImage();
	void InitializeAchievements();
	void DeactivateCFAButtons();
	void ActivateCFAButtons();
	void InitializeMainMenuButtons();
	bool m_cfaTooltipShown[s_cfaButtonCount];
	bool m_ofaTooltipShown[s_ofaButtonCount];
	bool m_keyTooltipShown;
	float m_tooltipTimer;
	void RemoveTooltips();
	void RemoveAchievements();
	void RemoveCactusImage();

public:
	SphHUD(SphAvatar* avatar);
	void Update(float seconds);
	void ToggleConsole();
	bool ConsoleFocus();
	void ToggleMainMenu();
	bool MainMenuFocus();
	bool TooltipActive();
	void EnableCFAButton(SphAvatar::ClosedType type);
	void DisableCFAButton(SphAvatar::ClosedType type);
	void EnableOFAButton(SphAvatar::Attachment attachment);
	void DisableOFAButton(SphAvatar::Attachment attachment);
	void EnableKeyButton(SphKey::keyType key);
	void DisableKeyButton(SphKey::keyType key);
	void EnableCactusImage(int totalCacti);
	void EnableAchievement(int achievementNumber);
	int ClickCFAButton(float mouseX, float mouseY);
	int ClickMainMenuButton(float mouseX, float mouseY);
	void Quit();
	void Reset();

};