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
	AddOEventHandler("BattleEnd", bind(&UIBattleWindow::BattleEnd, this, placeholders::_1));
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
	isVisible = true;
	if (typeid(*lpCaller) == typeid(BattleField))
	{
		lpBattleField = (BattleField*)lpCaller;

		int index = 0, group = 0;
		if (lpBattleField->GetHeroParty().size() == 1)
		{
			// 네크로멘서는 무조건 여길 타선 안된다.
			UIBattleUnit* lpUIBattleUnit = GameUI::CreateUI<UIBattleUnit>(this);
			lpUIBattleUnit->Init(UI_ANCHOR::BOTTOM_MIDDLE, { -135.0f, 150.0f }, lpBattleField->GetHeroParty().back()->GetWidth(), lpBattleField->GetHeroParty().back()->GetHeight());
			lpUIBattleUnit->SetBattleUnit(lpBattleField->GetHeroParty().back());
		}
		else
		{
			for (BattleUnit*& lpBattleUnit : lpBattleField->GetHeroParty())
			{
				UIBattleUnit* lpUIBattleUnit = GameUI::CreateUI<UIBattleUnit>();
				if (index % 2 == 0) InsertChild(lpUIBattleUnit, 0);
				else AddChildUI(lpUIBattleUnit);
				lpUIBattleUnit->Init(UI_ANCHOR::BOTTOM_MIDDLE, { -65.0f - (group % 2) * 140.0f, 150.0f + 60.0f * (1 + (-2) * (index % 2)) * group }, lpBattleUnit->GetWidth(), lpBattleUnit->GetHeight());
				lpUIBattleUnit->SetBattleUnit(lpBattleUnit);

				if (index % 2 == 0) ++group;
				++index;
			}
		}
		
		index = 0;
		group = 0;
		if (lpBattleField->GetEnemyParty().size() == 1)
		{
			UIBattleUnit* lpUIBattleUnit = GameUI::CreateUI<UIBattleUnit>(this);
			lpUIBattleUnit->Init(UI_ANCHOR::BOTTOM_MIDDLE, { 135.0f, 150.0f }, lpBattleField->GetEnemyParty().back()->GetWidth(), lpBattleField->GetEnemyParty().back()->GetHeight());
			lpUIBattleUnit->SetBattleUnit(lpBattleField->GetEnemyParty().back());
		}
		else
		{
			for (BattleUnit*& lpBattleUnit : lpBattleField->GetEnemyParty())
			{
				UIBattleUnit* lpUIBattleUnit = GameUI::CreateUI<UIBattleUnit>();
				if (index % 2 == 0) InsertChild(lpUIBattleUnit, 0);
				else AddChildUI(lpUIBattleUnit);
				lpUIBattleUnit->Init(UI_ANCHOR::BOTTOM_MIDDLE, { 65.0f + (group % 2) * 140.0f, 150.0f + 60.0f * (1 + (-2) * (index % 2)) * group }, lpBattleUnit->GetWidth(), lpBattleUnit->GetHeight());
				lpUIBattleUnit->SetBattleUnit(lpBattleUnit);

				if (index % 2 == 0) ++group;
				++index;
			}
		}
	}
}

void UIBattleWindow::BattleEnd(ObserverHandler* lpCaller)
{
	isVisible = false;

	for (int i = 0; i < vChildUI.size(); ++i)
	{
		vChildUI[i]->Release();
	}
	vChildUI.clear();
}
