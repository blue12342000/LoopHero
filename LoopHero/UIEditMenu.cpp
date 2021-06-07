#include "UIEditMenu.h"
#include "UIButton.h"
#include "Image.h"

void UIEditMenu::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpBackground = ImageManager::GetSingleton()->FindImage("edit_menu_background");

	lpButton[(int)EDIT_MENU::TITLE] = GameUI::Create<UIButton>(this);
	lpButton[(int)EDIT_MENU::TITLE]->Init(UI_ANCHOR::RIGHT_MIDDLE, {230.0f, 0.0f}, 80, 34, UI_BUTTON_TYPE::RADIO);
	lpButton[(int)EDIT_MENU::TITLE]->SetButtonImage("title_radio_button_40_17");
	lpButton[(int)EDIT_MENU::TITLE]->SetRadioGroup("edit_ui");
	lpButton[(int)EDIT_MENU::TITLE]->SetState(UI_BUTTON_STATE::PRESS);

	lpButton[(int)EDIT_MENU::INGAME] = GameUI::Create<UIButton>(this);
	lpButton[(int)EDIT_MENU::INGAME]->Init(UI_ANCHOR::RIGHT_MIDDLE, { 125.0f, 0.0f }, 100, 34, UI_BUTTON_TYPE::RADIO);
	lpButton[(int)EDIT_MENU::INGAME]->SetButtonImage("ingame_radio_button_50_17");
	lpButton[(int)EDIT_MENU::INGAME]->SetRadioGroup("edit_ui");

	lpButton[(int)EDIT_MENU::EXIT] = GameUI::Create<UIButton>(this);
	lpButton[(int)EDIT_MENU::EXIT]->Init(UI_ANCHOR::RIGHT_MIDDLE, { 40.0f, 0.0f }, 80, 34, UI_BUTTON_TYPE::BUTTON);
	lpButton[(int)EDIT_MENU::EXIT]->SetButtonImage("exit_button_40_17");
}

void UIEditMenu::Release()
{
	GameUI::Release();
}

void UIEditMenu::Render(HDC hdc)
{
	if (lpBackground) lpBackground->Render(hdc, rc.left, rc.top, 0);

	GameUI::Render(hdc);
}

void UIEditMenu::SetEvent(EDIT_MENU scene, function<void()> func)
{
	lpButton[(int)scene]->ClearFunc();
	lpButton[(int)scene]->PushBackFunc(move(func));
}
