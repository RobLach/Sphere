#include "SphMath.h"
#include "SphSdlInput.h"
#include "SphHUD.h"
#include "SphGame.h"
#include "SphCamera.h"
#include "SphGui.h"
#include "SphSdl.h"

cfloat s_displayTime = 3.0; // seconds to display tooltip in HUD

// Initialize SphHUD
SphHUD::SphHUD(SphAvatar* avatar){
	this->m_avatar = avatar;

	yscale = (float)g_sdl.GetScreenHeight()/(float)g_sdl.GetScreenWidth();
	g_sdlInput.MouseControlOff();

	// Initialize Health bar
	m_health = new SphGuiText(8);
	Set(m_health->m_left, 0.04f * yscale, 0.89f);
	Set(m_health->m_right, 0.09f * yscale, 0.94f);
	this->UpdateHealth();
	g_gui.AddGuiObject(m_health);

	// Initialize Health Background
	m_activeCFA = new SphGuiImage();
	Set(m_activeCFA->m_left, 0.0f, 0.84f);
	Set(m_activeCFA->m_right, 0.16f * yscale, 1.0f);
	m_activeCFA->m_drawFrame = false;
	g_gui.AddGuiObject(m_activeCFA);

	// Initialize Console
	m_console = new SphGuiTextbox(256);
	Set(m_console->m_left, 0.0f, 0.0f);
	Set(m_console->m_right, 1.0f, 0.1f);
	g_gui.AddGuiObject(m_console);

	InitializeCFAButtons();
	InitializeOFAButtons();
	InitializeKeyButtons();
	InitializeTooltips();
	InitializeCactusImage();
	InitializeAchievements();

	SetN(m_cfaTooltipShown, false, s_cfaButtonCount);
	SetN(m_ofaTooltipShown, false, s_ofaButtonCount);
	m_keyTooltipShown = false;

	this->ActivateCFAButtons();

	InitializeMainMenuButtons();
	m_mainmenuActive = false;
}

//Update SphHUD.
//seconds - since the last update.
void SphHUD::Update(float seconds){
	//Enable/Disable Main Menu
	if(g_sdlInput.KeyDownEvent(SDLK_ESCAPE)){
		ToggleMainMenu();
	}

	//Enable/Disable console with '`' (the tilde key)
	if(g_sdlInput.KeyDownEvent(SDLK_BACKQUOTE) && !MainMenuFocus() && !TooltipActive()){
		ToggleConsole();
	}
	if(g_sdlInput.KeyDownEvent(SDLK_RETURN) && m_console->m_hasFocus){
		ParseConsoleInput();
		ConsoleOff();
	}

	//Clear tooltip
	if(TooltipActive()){
		m_tooltipTimer -= seconds;
		if(m_tooltipTimer < 0){
			RemoveTooltips();
			RemoveAchievements();
			m_tooltipActive = false;
		}
	}

	//Activate clicked CFA button
	int button = -1;
	if(g_sdlInput.MouseButtonDownEvent(g_sdlInput.s_leftMouseButton)){
		
		//Change as long as main menu or console is not active
		if(!(m_mainmenuActive || m_console->m_hasFocus)){
			button = ClickCFAButton((float)g_sdlInput.GetMouseX()/(float)g_sdl.GetScreenWidth(),
									(float)g_sdlInput.GetMouseY()/(float)g_sdl.GetScreenHeight());
			
			switch(button){
				case 0: m_avatar->SetClosedType(SphAvatar::Normal); break;
				case 1: m_avatar->SetClosedType(SphAvatar::Speed); break;
				case 2: m_avatar->SetClosedType(SphAvatar::Rubber); break;
				case 3: m_avatar->SetClosedType(SphAvatar::Metal); break;
				default: break;
			}
		}
		//Execute main menu commands
		else if(m_mainmenuActive){
			button = ClickMainMenuButton((float)g_sdlInput.GetMouseX()/(float)g_sdl.GetScreenWidth(),
										 (float)g_sdlInput.GetMouseY()/(float)g_sdl.GetScreenHeight());
			
			switch(button){
				case 1: g_game.Reset(); break;
				case 2: g_game.Quit(); g_game.m_exit = true; break;
				default: break;
			}
		}
	}

	UpdateHealth();
	ActivateCFAButtons();
}
// Initialize CFA Buttons in HUD
void SphHUD::InitializeCFAButtons(){
	// Initialize Normal CFA Button
	m_cfaButtons[m_avatar->Normal] = new SphGuiImage();
	Set(m_cfaButtons[m_avatar->Normal]->m_left, 0.01f, 0.74f);
	Set(m_cfaButtons[m_avatar->Normal]->m_right, 0.09f * yscale, 0.82f);
	m_cfaButtons[m_avatar->Normal]->m_drawFrame = false;
	m_cfaButtons[m_avatar->Normal]->Load("GUI/", "NormalSphere.tga");
	
	// Initialize Rubber CFA Button
	m_cfaButtons[m_avatar->Rubber] = new SphGuiImage();
	Set(m_cfaButtons[m_avatar->Rubber]->m_left, 0.01f, 0.65f);
	Set(m_cfaButtons[m_avatar->Rubber]->m_right, 0.09f * yscale, 0.73f);
	m_cfaButtons[m_avatar->Rubber]->m_drawFrame = false;
	m_cfaButtons[m_avatar->Rubber]->Load("GUI/", "RubberSphere.tga");

	// Initialize Speed CFA Button
	m_cfaButtons[m_avatar->Speed] = new SphGuiImage();
	Set(m_cfaButtons[m_avatar->Speed]->m_left, 0.01f, 0.56f);
	Set(m_cfaButtons[m_avatar->Speed]->m_right, 0.09f * yscale, 0.64f);
	m_cfaButtons[m_avatar->Speed]->m_drawFrame = false;
	m_cfaButtons[m_avatar->Speed]->Load("GUI/", "SpeedSphere.tga");

	// Initialize Metal CFA Button
	m_cfaButtons[m_avatar->Metal] = new SphGuiImage();
	Set(m_cfaButtons[m_avatar->Metal]->m_left, 0.01f, 0.47f);
	Set(m_cfaButtons[m_avatar->Metal]->m_right, 0.09f * yscale, 0.55f);
	m_cfaButtons[m_avatar->Metal]->m_drawFrame = false;
	m_cfaButtons[m_avatar->Metal]->Load("GUI/", "MetalSphere.tga");
}

// Initialize OFA Buttons in HUD
void SphHUD::InitializeOFAButtons(){
	// Initialize Laser Gun OFA Button
	m_ofaButtons[m_avatar->LaserGun] = new SphGuiImage();
	Set(m_ofaButtons[m_avatar->LaserGun]->m_left, 0.18f * yscale, 0.90f);
	Set(m_ofaButtons[m_avatar->LaserGun]->m_right, 0.26f * yscale, 0.98f);
	m_ofaButtons[m_avatar->LaserGun]->m_drawFrame = false;
	m_ofaButtons[m_avatar->LaserGun]->m_frameLineWidth = 3.0f;
	m_ofaButtons[m_avatar->LaserGun]->Load("GUI/", "laser.tga");

	// Initialize Spray OFA Button
	m_ofaButtons[m_avatar->SprayGun] = new SphGuiImage();
	Set(m_ofaButtons[m_avatar->SprayGun]->m_left, 0.27f * yscale, 0.90f);
	Set(m_ofaButtons[m_avatar->SprayGun]->m_right, 0.35f * yscale, 0.98f);
	m_ofaButtons[m_avatar->SprayGun]->m_drawFrame = false;
	m_ofaButtons[m_avatar->SprayGun]->m_frameLineWidth = 3.0f;
	m_ofaButtons[m_avatar->SprayGun]->Load("GUI/", "laser.tga");
	
	// Initialize Legs OFA Button
	m_ofaButtons[m_avatar->Legs] = new SphGuiImage();
	Set(m_ofaButtons[m_avatar->Legs]->m_left, 0.36f * yscale, 0.90f);
	Set(m_ofaButtons[m_avatar->Legs]->m_right, 0.44f * yscale, 0.98f);
	m_ofaButtons[m_avatar->Legs]->m_drawFrame = false;
	m_ofaButtons[m_avatar->Legs]->m_frameLineWidth = 3.0f;
	m_ofaButtons[m_avatar->Legs]->Load("GUI/", "legs.tga");
}

// Initialize Key Buttons in HUD
void SphHUD::InitializeKeyButtons(){
	// Initialize Red Key Button
	m_keyButtons[SphKey::Red] = new SphGuiImage();
	Set(m_keyButtons[SphKey::Red]->m_left, 0.98f - (0.06f * yscale), 0.92f);
	Set(m_keyButtons[SphKey::Red]->m_right, 0.98f, 0.98f);
	m_keyButtons[SphKey::Red]->m_drawFrame = false;
	m_keyButtons[SphKey::Red]->Load("GUI/", "redkey.tga");

	// Initialize Yellow Key Button
	m_keyButtons[SphKey::Yellow] = new SphGuiImage();
	Set(m_keyButtons[SphKey::Yellow]->m_left, 0.91f - (0.06f * yscale), 0.92f);
	Set(m_keyButtons[SphKey::Yellow]->m_right, 0.91f, 0.98f);
	m_keyButtons[SphKey::Yellow]->m_drawFrame = false;
	m_keyButtons[SphKey::Yellow]->Load("GUI/", "yellowkey.tga");
}

// Initialize Main Menu Buttons in HUD
void SphHUD::InitializeMainMenuButtons(){
	// Initialize Level Reset Button
	m_mainmenuButtons[1] = new SphGuiImage();
	Set(m_mainmenuButtons[1]->m_left, 0.52f, 0.72f);
	Set(m_mainmenuButtons[1]->m_right, 0.81f, 0.81f);
	
	// Initialize Quit Button
	m_mainmenuButtons[2] = new SphGuiImage();
	Set(m_mainmenuButtons[2]->m_left, 0.22f, 0.80f);
	Set(m_mainmenuButtons[2]->m_right, 0.51f, 0.88f);

	// Initialize Main Menu
	m_mainmenuButtons[0] = new SphGuiImage();
	Set(m_mainmenuButtons[0]->m_left, 0.0f, 0.0f);
	Set(m_mainmenuButtons[0]->m_right, 1.0f, 1.0f);
	m_mainmenuButtons[0]->Load("GUI/", "MainMenu.tga");
}

// Initialize all tooltips
void SphHUD::InitializeTooltips(){
	// Initialize Normal CFA Tooltip
	m_cfaTooltip[m_avatar->Normal] = new SphGuiText(70);
	Set(m_cfaTooltip[m_avatar->Normal]->m_left, 0.14f, 0.0f);
	Set(m_cfaTooltip[m_avatar->Normal]->m_right, 0.86f, 0.1f);
	m_cfaTooltip[m_avatar->Normal]->m_drawFrame = false;
	m_cfaTooltip[m_avatar->Normal]->m_frameLineWidth = 4.0f;
	strcpy(m_cfaTooltip[m_avatar->Normal]->m_text, "Move with W,A,S,D keys. Open and close with TAB.\nClick to Continue...");
	
	// Initialize Speed CFA Tooltip
	m_cfaTooltip[m_avatar->Speed] = new SphGuiText(44);
	Set(m_cfaTooltip[m_avatar->Speed]->m_left, 0.19f, 0.0f);
	Set(m_cfaTooltip[m_avatar->Speed]->m_right, 0.81f, 0.05f);
	m_cfaTooltip[m_avatar->Speed]->m_drawFrame = false;
	m_cfaTooltip[m_avatar->Speed]->m_frameLineWidth = 4.0f;
	strcpy(m_cfaTooltip[m_avatar->Speed]->m_text, "Use the Speed Ability to move twice as fast.");

	// Initialize Rubber CFA Tooltip
	m_cfaTooltip[m_avatar->Rubber] = new SphGuiText(57);
	Set(m_cfaTooltip[m_avatar->Rubber]->m_left, 0.09f, 0.0f);
	Set(m_cfaTooltip[m_avatar->Rubber]->m_right, 0.91f, 0.05f);
	m_cfaTooltip[m_avatar->Rubber]->m_drawFrame = false;
	m_cfaTooltip[m_avatar->Rubber]->m_frameLineWidth = 4.0f;
	strcpy(m_cfaTooltip[m_avatar->Rubber]->m_text, "Use the Rubber Ability to bounce back to the same height.");
	
	// Initialize Metal CFA Tooltip
	m_cfaTooltip[m_avatar->Metal] = new SphGuiText(52);
	Set(m_cfaTooltip[m_avatar->Metal]->m_left, 0.14f, 0.0f);
	Set(m_cfaTooltip[m_avatar->Metal]->m_right, 0.86f, 0.05f);
	m_cfaTooltip[m_avatar->Metal]->m_drawFrame = false;
	m_cfaTooltip[m_avatar->Metal]->m_frameLineWidth = 4.0f;
	strcpy(m_cfaTooltip[m_avatar->Metal]->m_text, "Use the Metal Ability to stick to magnetic surfaces.");

	// Initialize Laser Gun OFA Tooltip
	m_ofaTooltip[m_avatar->LaserGun] = new SphGuiText(40);
	Set(m_ofaTooltip[m_avatar->LaserGun]->m_left, 0.21f, 0.0f);
	Set(m_ofaTooltip[m_avatar->LaserGun]->m_right, 0.79f, 0.05f);
	m_ofaTooltip[m_avatar->LaserGun]->m_drawFrame = false;
	m_ofaTooltip[m_avatar->LaserGun]->m_frameLineWidth = 4.0f;
	strcpy(m_ofaTooltip[m_avatar->LaserGun]->m_text, "Use the CTRL key to fire the Laser Gun.");

	// Initialize Spray OFA Tooltip
	m_ofaTooltip[m_avatar->SprayGun] = new SphGuiText(42);
	Set(m_ofaTooltip[m_avatar->SprayGun]->m_left, 0.19f, 0.0f);
	Set(m_ofaTooltip[m_avatar->SprayGun]->m_right, 0.81f, 0.05f);
	m_ofaTooltip[m_avatar->SprayGun]->m_drawFrame = false;
	m_ofaTooltip[m_avatar->SprayGun]->m_frameLineWidth = 4.0f;
	strcpy(m_ofaTooltip[m_avatar->SprayGun]->m_text, "Use the Spray Gun to defeat flying swarms.");
	
	// Initialize Legs OFA Tooltip
	m_ofaTooltip[m_avatar->Legs] = new SphGuiText(26);
	Set(m_ofaTooltip[m_avatar->Legs]->m_left, 0.29f, 0.0f);
	Set(m_ofaTooltip[m_avatar->Legs]->m_right, 0.71f, 0.05f);
	m_ofaTooltip[m_avatar->Legs]->m_drawFrame = false;
	m_ofaTooltip[m_avatar->Legs]->m_frameLineWidth = 4.0f;
	strcpy(m_ofaTooltip[m_avatar->Legs]->m_text, " Use the SPACEBAR to jump.");

	// Initialize Red Key Tooltip
	m_keyTooltip = new SphGuiText(52);
	Set(m_keyTooltip->m_left, 0.13f, 0.0f);
	Set(m_keyTooltip->m_right, 0.87f, 0.05f);
	m_keyTooltip->m_drawFrame = false;
	m_keyTooltip->m_frameLineWidth = 4.0f;
	strcpy(m_keyTooltip->m_text, "Use the colored keys to open the same colored chest.");

}

// Initialize all cacti.
void SphHUD::InitializeCactusImage(){

	m_cacti[0] = new SphGuiImage();
	Set(m_cacti[0]->m_left, 0.98f - (0.08f * yscale), 0.01f);
	Set(m_cacti[0]->m_right, 0.98f, 0.09f);
	m_cacti[0]->m_drawFrame = false;
	m_cacti[0]->Load("GUI/", "cactus.tga");

	m_cacti[1] = new SphGuiImage();
	Set(m_cacti[1]->m_left, 0.98f - (0.08f * yscale), 0.10f);
	Set(m_cacti[1]->m_right, 0.98f, 0.18f);
	m_cacti[1]->m_drawFrame = false;
	m_cacti[1]->Load("GUI/", "cactus.tga");

	m_cacti[2] = new SphGuiImage();
	Set(m_cacti[2]->m_left, 0.98f - (0.08f * yscale), 0.19f);
	Set(m_cacti[2]->m_right, 0.98f, 0.27f);
	m_cacti[2]->m_drawFrame = false;
	m_cacti[2]->Load("GUI/", "cactus.tga");

	m_cacti[3] = new SphGuiImage();
	Set(m_cacti[3]->m_left, 0.98f - (0.08f * yscale), 0.28f);
	Set(m_cacti[3]->m_right, 0.98f, 0.36f);
	m_cacti[3]->m_drawFrame = false;
	m_cacti[3]->Load("GUI/", "cactus.tga");

	m_cacti[4] = new SphGuiImage();
	Set(m_cacti[4]->m_left, 0.98f - (0.08f * yscale), 0.37f);
	Set(m_cacti[4]->m_right, 0.98f, 0.45f);
	m_cacti[4]->m_drawFrame = false;
	m_cacti[4]->Load("GUI/", "cactus.tga");

	m_cacti[5] = new SphGuiImage();
	Set(m_cacti[5]->m_left, 0.98f - (0.08f * yscale), 0.46f);
	Set(m_cacti[5]->m_right, 0.98f, 0.54f);
	m_cacti[5]->m_drawFrame = false;
	m_cacti[5]->Load("GUI/", "cactus.tga");

	m_cacti[6] = new SphGuiImage();
	Set(m_cacti[6]->m_left, 0.98f - (0.08f * yscale), 0.55f);
	Set(m_cacti[6]->m_right, 0.98f, 0.63f);
	m_cacti[6]->m_drawFrame = false;
	m_cacti[6]->Load("GUI/", "cactus.tga");

}

// Initialize all achievements
void SphHUD::InitializeAchievements(){

	m_achievements[0] = new SphGuiImage();
	Set(m_achievements[0]->m_left, 0.30f, 0.10f);
	Set(m_achievements[0]->m_right, 1.00f * yscale, 0.20f);
	m_achievements[0]->m_drawFrame = false;
	m_achievements[0]->Load("GUI/", "cactiAchievement.tga");

}

// Update Health value in HUD
void SphHUD::UpdateHealth(){
	_itoa(m_avatar->GetHealth(), m_health->m_text, 10);
}

// Toggle console
void SphHUD::ToggleConsole(){
	if(!m_console->m_hasFocus)
		ConsoleOn();
	else
		ConsoleOff();
}

// Enable console
void SphHUD::ConsoleOn(){
	m_console->m_hasFocus = true;
	m_console->m_drawFrame = true;
	g_sdlInput.MouseControlOn();
}
// Disable console
void SphHUD::ConsoleOff(){
	m_console->Reset();
	g_sdlInput.MouseControlOff();
}

// Check if Console has focus
bool SphHUD::ConsoleFocus(){
	return m_console->m_hasFocus;
}

// Deactivate all CFA Buttons
void SphHUD::DeactivateCFAButtons(){
	for(int index = 0; index < s_cfaButtonCount; index++){
		m_cfaButtons[index]->m_frameLineWidth = 1.0f;
	}
}

// Activate specific CFA Button
void SphHUD::ActivateCFAButtons(){
	this->DeactivateCFAButtons();
	m_cfaButtons[m_avatar->GetClosedType()]->m_frameLineWidth = 3.0f;
	m_activeCFA->m_texture = m_cfaButtons[m_avatar->GetClosedType()]->m_texture;
}

// Check if CFA HUD button is clicked
// returns index of CFA button if true otherwise -1
int SphHUD::ClickCFAButton(float mouseX, float mouseY){
	int clicked = -1;
	for(int index = 0; index < s_cfaButtonCount; index++){
		if((m_cfaButtons[index]->m_left[O_X] <= mouseX) && (mouseX <= m_cfaButtons[index]->m_right[O_X])){
			if((m_cfaButtons[index]->m_left[O_Y] <= mouseY) && (mouseY <= m_cfaButtons[index]->m_right[O_Y]))
				clicked = index;
		}
	}
	return clicked;
}

// Check if CFA HUD button is clicked
// returns index of CFA button if true otherwise -1
int SphHUD::ClickMainMenuButton(float mouseX, float mouseY){
	int clicked = -1;
	for(int index = 1; index < 3; index++){
		if((m_mainmenuButtons[index]->m_left[O_X] <= mouseX) && (mouseX <= m_mainmenuButtons[index]->m_right[O_X])){
			if((m_mainmenuButtons[index]->m_left[O_Y] <= mouseY) && (mouseY <= m_mainmenuButtons[index]->m_right[O_Y]))
				clicked = index;
		}
	}
	return clicked;
}
// Toggle Main Menu
void SphHUD::ToggleMainMenu(){
	if(!m_mainmenuActive)
		MainMenuOn();
	else
		MainMenuOff();
}

// Active Main Menu
void SphHUD::MainMenuOn(){
	g_gui.AddGuiObject(m_mainmenuButtons[1]);
	g_gui.AddGuiObject(m_mainmenuButtons[2]);
	g_gui.AddGuiObject(m_mainmenuButtons[0]);
	g_sdlInput.MouseControlOn();
	m_mainmenuActive = true;
}

// Deactive Main Menu
void SphHUD::MainMenuOff(){
	g_gui.RemoveGuiObject(m_mainmenuButtons[1]);
	g_gui.RemoveGuiObject(m_mainmenuButtons[2]);
	g_gui.RemoveGuiObject(m_mainmenuButtons[0]);
	g_sdlInput.MouseControlOff();
	m_mainmenuActive = false;
}
// Check if Main Menu has focus
bool SphHUD::MainMenuFocus(){
	return m_mainmenuActive;
}
// Draw CFA in HUD
void SphHUD::EnableCFAButton(SphAvatar::ClosedType type){
	if(!m_cfaTooltipShown[type]){
		m_cfaTooltipShown[type] = true;
		m_tooltipActive = true;
		m_tooltipTimer = s_displayTime;
		RemoveTooltips();
		g_gui.AddGuiObject(m_cfaTooltip[type]);
	}
	g_gui.AddGuiObject(m_cfaButtons[type]);
}

// Erase CFA from HUD
void SphHUD::DisableCFAButton(SphAvatar::ClosedType type){
	g_gui.RemoveGuiObject(m_cfaButtons[type]);
}

// Draw OFA in HUD
void SphHUD::EnableOFAButton(SphAvatar::Attachment attachment){
	if(!m_ofaTooltipShown[attachment]){
		m_ofaTooltipShown[attachment] = true;
		m_tooltipActive = true;
		m_tooltipTimer = s_displayTime;
		RemoveTooltips();
		g_gui.AddGuiObject(m_ofaTooltip[attachment]);
	}
	g_gui.AddGuiObject(m_ofaButtons[attachment]);
}

// Erase CFA from HUD
void SphHUD::DisableOFAButton(SphAvatar::Attachment attachment){
	g_gui.RemoveGuiObject(m_ofaButtons[attachment]);
}

// Draw Gears in HUD
void SphHUD::EnableKeyButton(SphKey::keyType color){
	if(!m_keyTooltipShown){
		m_keyTooltipShown = true;
		m_tooltipActive = true;
		m_tooltipTimer = s_displayTime;
		RemoveTooltips();
		g_gui.AddGuiObject(m_keyTooltip);
	}
	g_gui.AddGuiObject(m_keyButtons[color]);
}

// Erase CFA from HUD
void SphHUD::DisableKeyButton(SphKey::keyType color){
	g_gui.RemoveGuiObject(m_keyButtons[color]);
}

// Check if a tooltip is active
bool SphHUD::TooltipActive(){
	return m_tooltipActive;
}

// Clear tooltips from the hud
void SphHUD::RemoveTooltips(){
	g_gui.RemoveGuiObject(m_cfaTooltip[m_avatar->Normal]);
	g_gui.RemoveGuiObject(m_cfaTooltip[m_avatar->Speed]);
	g_gui.RemoveGuiObject(m_cfaTooltip[m_avatar->Rubber]);
	g_gui.RemoveGuiObject(m_cfaTooltip[m_avatar->Metal]);

	g_gui.RemoveGuiObject(m_ofaTooltip[m_avatar->LaserGun]);
	g_gui.RemoveGuiObject(m_ofaTooltip[m_avatar->SprayGun]);
	g_gui.RemoveGuiObject(m_ofaTooltip[m_avatar->Legs]);

	g_gui.RemoveGuiObject(m_keyTooltip);
}

void SphHUD::EnableCactusImage(int totalCacti){
	g_gui.AddGuiObject(m_cacti[totalCacti]);
}

void SphHUD::RemoveCactusImage(){
	g_gui.RemoveGuiObject(m_cacti[0]);
	g_gui.RemoveGuiObject(m_cacti[1]);
	g_gui.RemoveGuiObject(m_cacti[2]);
	g_gui.RemoveGuiObject(m_cacti[3]);
	g_gui.RemoveGuiObject(m_cacti[4]);
	g_gui.RemoveGuiObject(m_cacti[5]);
	g_gui.RemoveGuiObject(m_cacti[6]);

}

void SphHUD::EnableAchievement(int achievementNumber){
	m_tooltipActive = true;
	m_tooltipTimer = s_displayTime;
	RemoveAchievements();
	g_gui.AddGuiObject(m_achievements[achievementNumber]);
}

void SphHUD::RemoveAchievements(){
	g_gui.RemoveGuiObject(m_achievements[0]);
}

// Parse console input
void SphHUD::ParseConsoleInput(){
	if(m_console->IsInputFinished()){
		char* command = strtok(m_console->m_text, " ");
		if(command == NULL) return;

		if(strcmp(command, "give_smallMedpack") == 0)
			m_avatar->ChangeHealth(30);
		else if(strcmp(command, "give_mediumMedpack") == 0)
			m_avatar->ChangeHealth(60);
		else if(strcmp(command, "give_fullMedpack") == 0)
			m_avatar->ChangeHealth(100);
		else if(strcmp(command, "take_smallMedpack") == 0)
			m_avatar->ChangeHealth(-30);
		else if(strcmp(command, "take_mediumMedpack") == 0)
			m_avatar->ChangeHealth(-60);
		else if(strcmp(command, "take_fullMedpack") == 0)
			m_avatar->ChangeHealth(-100);

		else if(strcmp(command, "addClosed_Normal") == 0)
			m_avatar->AddClosedType(SphAvatar::Normal);
		else if(strcmp(command, "addClosed_Speed") == 0)
			m_avatar->AddClosedType(SphAvatar::Speed);
		else if(strcmp(command, "addClosed_Rubber") == 0)
			m_avatar->AddClosedType(SphAvatar::Rubber);
		else if(strcmp(command, "addClosed_Metal") == 0)
			m_avatar->AddClosedType(SphAvatar::Metal);
		else if(strcmp(command, "addClosed_all") == 0){
			m_cfaTooltipShown[SphAvatar::Normal] = true;
			m_cfaTooltipShown[SphAvatar::Speed] = true;
			m_cfaTooltipShown[SphAvatar::Rubber] = true;
			m_cfaTooltipShown[SphAvatar::Metal] = true;

			m_avatar->AddClosedType(SphAvatar::Normal);
			m_avatar->AddClosedType(SphAvatar::Speed);
			m_avatar->AddClosedType(SphAvatar::Rubber);
			m_avatar->AddClosedType(SphAvatar::Metal);
		}

		else if(strcmp(command, "rmClosed_Speed") == 0)
			m_avatar->RemoveClosedType(SphAvatar::Speed);
		else if(strcmp(command, "rmClosed_Rubber") == 0)
			m_avatar->RemoveClosedType(SphAvatar::Rubber);
		else if(strcmp(command, "rmClosed_Metal") == 0)
			m_avatar->RemoveClosedType(SphAvatar::Metal);
		else if(strcmp(command, "rmClosed_all") == 0){
			m_avatar->RemoveClosedType(SphAvatar::Speed);
			m_avatar->RemoveClosedType(SphAvatar::Rubber);
			m_avatar->RemoveClosedType(SphAvatar::Metal);
		}

		else if(strcmp(command, "addOpen_LaserGun") == 0)
			m_avatar->AddAttachment(SphAvatar::LaserGun);
		else if(strcmp(command, "addOpen_SprayGun") == 0)
			m_avatar->AddAttachment(SphAvatar::SprayGun);
		else if(strcmp(command, "addOpen_Legs") == 0)
			m_avatar->AddAttachment(SphAvatar::Legs);
		else if(strcmp(command, "addOpen_all") == 0){
			m_ofaTooltipShown[SphAvatar::LaserGun] = true;
			m_ofaTooltipShown[SphAvatar::SprayGun] = true;
			m_ofaTooltipShown[SphAvatar::Legs] = true;

			m_avatar->AddAttachment(SphAvatar::LaserGun);
			m_avatar->AddAttachment(SphAvatar::SprayGun);
			m_avatar->AddAttachment(SphAvatar::Legs);
		}
		
		else if(strcmp(command, "rmOpen_LaserGun") == 0)
			m_avatar->RemoveAttachment(SphAvatar::LaserGun);
		else if(strcmp(command, "rmOpen_SprayGun") == 0)
			m_avatar->RemoveAttachment(SphAvatar::SprayGun);
		else if(strcmp(command, "rmOpen_Legs") == 0)
			m_avatar->RemoveAttachment(SphAvatar::Legs);
		else if(strcmp(command, "rmOpen_all") == 0){
			m_avatar->RemoveAttachment(SphAvatar::LaserGun);
			m_avatar->RemoveAttachment(SphAvatar::SprayGun);
			m_avatar->RemoveAttachment(SphAvatar::Legs);
		}

		else if(strcmp(command, "addKey_Red") == 0)
			m_avatar->AddKey(SphKey::Red);
		else if(strcmp(command, "rmKey_Red") == 0)
			m_avatar->RemoveKey(SphKey::Red);

		else if(strcmp(command, "sv_reset") == 0){
			g_game.Reset();
		}
		else if(strcmp(command, "setPosition") == 0){ //command: setPosition x y z
		
			float position[N_XYZ];
			char* temp;
			for(int i = 0; (i < 3) && ((temp = strtok(NULL, " ")) != NULL); i++)
					position[i] = (float)atof(temp);

			if(temp != NULL)
			{
				m_avatar->SetPosition(position);
				m_avatar->SetVelocity(g_zeroes);
			}
		}
		else if(!strcmp(command, "wireframe")){
			int renderMode = (int)atoi(strtok(NULL, " "));
			g_game.SetRenderMode(renderMode);
		}
		else if(!strcmp(command, "invert_camera")){
			bool on = ((int)atoi(strtok(NULL, " ")) == 1);
			g_game.m_camera->SetInvertPitch(on);
		}
		else if(strcmp(command, "setonfire") == 0){
			m_avatar->SetOnFire();
		}
	}
}



//Quit SphHUD and release all resources
void SphHUD::Quit(){
	g_gui.RemoveGuiObject(m_health);
	g_gui.RemoveGuiObject(m_activeCFA);
	RemoveCactusImage();

	ConsoleOff();
	MainMenuOff();
}

void SphHUD::Reset(){
	this->Quit();
	g_gui.AddGuiObject(m_health);
	g_gui.AddGuiObject(m_activeCFA);
}