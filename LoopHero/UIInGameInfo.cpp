#include "UIInGameInfo.h"
#include "UITextField.h"
#include "Card.h"
#include "Unit.h"
#include "EquipItem.h"
#include "FieldTile.h"
#include "Utill.h"

void UIInGameInfo::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpTitle = GameUI::CreateUI<UITextField>(this);
	lpTitle->Init(UI_ANCHOR::TOP_MIDDLE, { 0.0f, 5.0f }, width, 30);
	lpTitle->SetFont(UI_TEXT_HALIGN::CENTER, UI_TEXT_VALIGN::MIDDLE, UI_TEXT_STYLE::BOLD, UI_TEXT_LINE::SINGLE, 15, RGB(255, 255, 255), "µ¸¿ò");
	//lpTitle->SetEventCatch(EVENT_CATCH::BLOCK_PASS);

	lpContent = GameUI::CreateUI<UITextField>(this);
	lpContent->Init(UI_ANCHOR::TOP_MIDDLE, { 0.0f, 50.0f }, width, height - 50);
	lpContent->SetFont(UI_TEXT_HALIGN::LEFT, UI_TEXT_VALIGN::TOP, UI_TEXT_STYLE::NORMAL, UI_TEXT_LINE::MULTI, 11, RGB(255, 255, 255), "µ¸¿ò");
	//lpContent->SetEventCatch(EVENT_CATCH::BLOCK_PASS);

	ObserverManager::GetSingleton()->RegisterObserver(this);
	AddOEventHandler("ViewInfo", bind(&UIInGameInfo::ViewInfo, this, placeholders::_1));
	AddOEventHandler("HideInfo", bind(&UIInGameInfo::HideInfo, this, placeholders::_1));
}

void UIInGameInfo::Render(HDC hdc)
{
	//RenderRectangle(hdc, rc, RGB(0, 0, 0));

	GameUI::Render(hdc);
}

void UIInGameInfo::SetTitle(string text)
{
	lpTitle->SetText(text);
}

void UIInGameInfo::SetContent(string text)
{
	lpContent->SetText(text);
}

void UIInGameInfo::ViewInfo(ObserverHandler* lpTarget)
{
	GameNode* lpNode = nullptr;
	if (typeid(*lpTarget) == typeid(Card))
	{
		lpNode = (Card*)lpTarget;
	}
	else if (typeid(*lpTarget) == typeid(Unit))
	{
		lpNode = (Unit*)lpTarget;
	}
	else if (typeid(*lpTarget) == typeid(EquipItem))
	{
		lpNode = (EquipItem*)lpTarget;
	}
	else if (typeid(*lpTarget) == typeid(FieldTile))
	{
		lpNode = (FieldTile*)lpTarget;
	}

	if (lpNode)
	{
		vector<string> vText = StringSplit(lpNode->ToString(), '|');
		SetTitle(vText[0]);
		if (vText.size() > 1) SetContent(vText[1]);
	}
}

void UIInGameInfo::HideInfo(ObserverHandler* lpTarget)
{
	lpTitle->SetText("");
	lpContent->SetText("");
}
