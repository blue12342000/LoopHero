#include "UITitleMenu.h"
#include "UIButton.h"
#include "UITextField.h"
#include "SceneManager.h"
#include "ImageManager.h"
#include "Image.h"

void UITitleMenu::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);
	CreateAnimController("title_menu");

	lpBackground = ImageManager::GetSingleton()->FindImage("title_menubackground");

	lpStartBtn = GameUI::Create<UIButton>(this);
	lpStartBtn->Init(UI_ANCHOR::BOTTOM_MIDDLE, { 0.0f, 260.0f }, 198, 34, UI_BUTTON_TYPE::BUTTON);
	lpStartBtn->SetButtonImage("button_99_17");
	if (GameData::GetSingleton()->IsPlayMode()) lpStartBtn->PushBackFunc(bind(&SceneManager::ChangeScene, SceneManager::GetSingleton(), SCENE_KIND::INGAME, LOADING_STYLE::FADE_OUT));

	UITextField* lpStartText = GameUI::Create<UITextField>(lpStartBtn);
	lpStartText->Init(UI_ANCHOR::MIDDLE, { 0.0f, 0.0f }, 198, 34);
	lpStartText->SetFont(UI_TEXT_HALIGN::CENTER, UI_TEXT_VALIGN::MIDDLE, UI_TEXT_LINE::SINGLE, 20, RGB(255, 255, 255), "B�����ٸ���");
	lpStartText->SetText("Start");
	lpStartText->SetEventCatch(EVENT_CATCH::PASS);

	lpEditBtn = GameUI::Create<UIButton>(this);
	lpEditBtn->Init(UI_ANCHOR::BOTTOM_MIDDLE, { 0.0f, 210.0f }, 198, 34, UI_BUTTON_TYPE::BUTTON);
	lpEditBtn->SetButtonImage("button_99_17");
	if (GameData::GetSingleton()->IsPlayMode()) lpEditBtn->PushBackFunc(bind(&SceneManager::ChangeScene, SceneManager::GetSingleton(), SCENE_KIND::ANIM_EDIT, LOADING_STYLE::FADE_OUT));

	UITextField* lpEditText = GameUI::Create<UITextField>(lpEditBtn);
	lpEditText->Init(UI_ANCHOR::MIDDLE, { 0.0f, 0.0f }, 198, 34);
	lpEditText->SetFont(UI_TEXT_HALIGN::CENTER, UI_TEXT_VALIGN::MIDDLE, UI_TEXT_LINE::SINGLE, 20, RGB(255, 255, 255), "B�����ٸ���");
	lpEditText->SetText("Edit");
	lpEditText->SetEventCatch(EVENT_CATCH::PASS);

	lpExitBtn = GameUI::Create<UIButton>(this);
	lpExitBtn->Init(UI_ANCHOR::BOTTOM_MIDDLE, { 0.0f, 160.0f }, 198, 34, UI_BUTTON_TYPE::BUTTON);
	lpExitBtn->SetButtonImage("button_99_17");
	if (GameData::GetSingleton()->IsPlayMode()) lpExitBtn->PushBackFunc(bind(&PostQuitMessage, 0));

	UITextField* lpExitText = GameUI::Create<UITextField>(lpExitBtn);
	lpExitText->Init(UI_ANCHOR::MIDDLE, { 0.0f, 0.0f }, 198, 34);
	lpExitText->SetFont(UI_TEXT_HALIGN::CENTER, UI_TEXT_VALIGN::MIDDLE, UI_TEXT_LINE::SINGLE, 20, RGB(255, 255, 255), "B�����ٸ���");
	lpExitText->SetText("Exit");
	lpExitText->SetEventCatch(EVENT_CATCH::PASS);
}

void UITitleMenu::Release()
{
	GameUI::Release();
}

void UITitleMenu::Render(HDC hdc)
{
	if (lpBackground)
	{
		lpBackground->Render(hdc, rc.left, rc.top, 0);
	}

	GameUI::Render(hdc);
}
