#include "UIItemInfo.h"
#include "UITextField.h"
#include "EquipItem.h"
#include "Image.h"
#include "Utill.h"

void UIItemInfo::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);
	SetEventCatch(EVENT_CATCH::BLOCK_PASS);

	lpBackground = ImageManager::GetSingleton()->FindImage("layer_background");
	lpTopDeco = ImageManager::GetSingleton()->FindImage("layer_background_top");
	lpBottomDeco = ImageManager::GetSingleton()->FindImage("layer_background_bottom");

	lpTag = GameUI::Create<UITextField>(this);
	lpTag->Init(UI_ANCHOR::LEFT_TOP, { 25.0f, 20.0f }, 80, 25);
	lpTag->SetFont(UI_TEXT_HALIGN::LEFT, UI_TEXT_VALIGN::MIDDLE, UI_TEXT_LINE::SINGLE, 17, RGB(255, 170, 255), "Lµ¸¿ò");

	lpName = GameUI::Create<UITextField>(this);
	lpName->Init(UI_ANCHOR::TOP_MIDDLE, { 0.0f, 20.0f }, width - 20, 25);
	lpName->SetFont(UI_TEXT_HALIGN::CENTER, UI_TEXT_VALIGN::MIDDLE, UI_TEXT_LINE::SINGLE, 20, RGB(255, 255, 255), "Bµ¸¿ò");

	lpContent = GameUI::Create<UITextField>(this);
	lpContent->Init(UI_ANCHOR::TOP_MIDDLE, { 0.0f, 50.0f }, width - 50, height - 50);
	lpContent->SetFont(UI_TEXT_HALIGN::LEFT, UI_TEXT_VALIGN::TOP, UI_TEXT_LINE::MULTI, 14, RGB(255, 255, 255), "Lµ¸¿ò");
}

void UIItemInfo::Release()
{
	GameUI::Release();
}

void UIItemInfo::Update(float deltaTime)
{
	GameUI::Update(deltaTime);
}

void UIItemInfo::Render(HDC hdc)
{
	if (lpBackground)
	{
		lpBackground->LoopRender(hdc, POINT{ rc.left, rc.top }, width, height, 0);
	}

	if (lpTopDeco)
	{
		lpTopDeco->Render(hdc, (rc.left + rc.right) / 2, rc.top, 0, IMAGE_ALIGN::MIDDLE_TOP);
	}

	if (lpBottomDeco)
	{
		lpBottomDeco->Render(hdc, (rc.left + rc.right) / 2, rc.bottom, 0, IMAGE_ALIGN::MIDDLE_BOTTOM);
	}

	GameUI::Render(hdc);
}

void UIItemInfo::SetEquipItem(EquipItem* lpEquipItem)
{
	vector<string> vStr = StringSplit(lpEquipItem->ToString(), '|');
	lpName->SetText(vStr[0]);
	lpContent->SetText(vStr[1]);
}

void UIItemInfo::SetTag(string text)
{
	if (lpTag) lpTag->SetText(text);
}
