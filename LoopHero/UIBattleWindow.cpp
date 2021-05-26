#include "UIBattleWindow.h"
#include "UIBattleUnit.h" 
#include "BattleUnit.h"
#include "BattleField.h"
#include "Animation.h"
#include "Image.h"
#include "Utill.h"

void UIBattleWindow::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);
	lpBackground = ImageManager::GetSingleton()->FindImage("battle_background");
	ObserverManager::GetSingleton()->RegisterObserver(this);
	AddOEventHandler("BattleStart", bind(&UIBattleWindow::BattleStart, this, placeholders::_1));
}

void UIBattleWindow::Release()
{
	GameUI::Release();
}

void UIBattleWindow::Update(float deltaTime)
{
	GameUI::Update(deltaTime);
}

void UIBattleWindow::Render(HDC hdc)
{
	RenderRectangle(hdc, rc, RGB(200, 200, 200));
	lpBackground->Render(hdc, rc.left, rc.top);

	GameUI::Render(hdc);
}

void UIBattleWindow::BattleStart(ObserverHandler* lpCaller)
{
	if (typeid(*lpCaller) == typeid(BattleField))
	{
		lpBattleField = (BattleField*)lpCaller;

		int index = 0;
		for (BattleUnit*& lpBattleUnit : lpBattleField->GetHeroParty())
		{
			UIBattleUnit* lpUIBattleUnit = GameUI::CreateUI<UIBattleUnit>(this);
			lpUIBattleUnit->Init(UI_ANCHOR::BOTTOM_MIDDLE, { -100.0f, 300.0f }, lpBattleUnit->GetWidth(), lpBattleUnit->GetHeight());
			lpUIBattleUnit->SetBattleUnit(lpBattleUnit);
		}
		
		index = 0;
		for (BattleUnit*& lpBattleUnit : lpBattleField->GetEnemyParty())
		{
			UIBattleUnit* lpUIBattleUnit = GameUI::CreateUI<UIBattleUnit>(this);
			lpUIBattleUnit->Init(UI_ANCHOR::BOTTOM_MIDDLE, { 100.0f, 300.0f }, lpBattleUnit->GetWidth(), lpBattleUnit->GetHeight());
			lpUIBattleUnit->SetBattleUnit(lpBattleUnit);
		}
	}
}

void UIBattleWindow::BattleEnd(ObserverHandler* lpCaller)
{
	isVisible = false;
	GameUI::Release();
}
