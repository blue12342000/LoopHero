#include "UIBattleUnit.h"
#include "UIProgressBar.h"
#include "UISprite.h"
#include "ImageManager.h"
#include "Image.h"
#include "BattleUnit.h"
#include "Unit.h"
#include "Utill.h"

void UIBattleUnit::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpSprite = GameUI::Create<UISprite>(this);
	lpSprite->Init(UI_ANCHOR::BOTTOM_MIDDLE, {0.0f, 0.0f}, width, height);
	lpSprite->SetEventCatch(EVENT_CATCH::PASS);

	lpHudBack = ImageManager::GetSingleton()->FindImage("battle_unit_statusbar");
	lpHpBar = GameUI::Create<UIProgressBar>(this);
	lpHpBar->Init(UI_ANCHOR::BOTTOM_MIDDLE, { 0.0f, 0.0f }, 50, 4, UI_BAR_TYPE::HORIZON, "", "battle_unit_statusbar_hp");

	lpActionBar = GameUI::Create<UIProgressBar>(this);
	lpActionBar->Init(UI_ANCHOR::BOTTOM_MIDDLE, { 0.0f, 0.0f }, 50, 2, UI_BAR_TYPE::HORIZON, "", "battle_unit_statusbar_action");

	lpHover = nullptr;

	action = 0;
	maxAction = 5;
}

void UIBattleUnit::Release()
{
	lpBattleUnit = nullptr;
	lpSprite->SetGameObject(nullptr);
	GameUI::Release();
}

void UIBattleUnit::Update(float deltaTime)
{
	if (lpBattleUnit)
	{
		lpHpBar->SetPos({ 0.0f, (float)lpBattleUnit->GetHeight() + 3.0f });
		lpActionBar->SetPos({ 0.0f, (float)lpBattleUnit->GetHeight() + 3.0f });
	}

	GameUI::Update(deltaTime);
}

void UIBattleUnit::Render(HDC hdc)
{
	//RenderRectangle(hdc, rc, RGB(0, 200, 200));

	if (lpBattleUnit)
	{
		lpHudBack->Render(hdc, origin.x + pos.x, rc.bottom - lpBattleUnit->GetHeight(), 0, IMAGE_ALIGN::MIDDLE_BOTTOM);
	}

	GameUI::Render(hdc);

	if (isHover && lpHover)
	{
		lpHover->Render(hdc, GetCenter().x, rc.bottom - lpBattleUnit->GetHeight() / 2, 0, IMAGE_ALIGN::CENTER);
	}
}

void UIBattleUnit::SetBattleUnit(BattleUnit* lpBattleUnit)
{
	this->lpBattleUnit = lpBattleUnit;

	if (lpBattleUnit)
	{
		lpSprite->SetGameObject(lpBattleUnit);
		if (lpBattleUnit->GetHeight() > 116)
		{
			lpHover = ImageManager::GetSingleton()->FindImage("battle_unit_hover_80_92");
		}
		else
		{
			lpHover = ImageManager::GetSingleton()->FindImage("battle_unit_hover_80_58");
		}

		lpHpBar->SetTrackingData(bind(&Unit::GetCurrHp, lpBattleUnit->GetUnit()));
		lpHpBar->SetTrackingMaxData(bind(&Unit::GetStatus, lpBattleUnit->GetUnit(), UNIT_STATUS::MAX_HP));
		lpActionBar->SetTrackingData(bind(&BattleUnit::GetAction, lpBattleUnit));
		lpActionBar->SetTrackingMaxData(bind(&BattleUnit::GetMaxAction, lpBattleUnit));
	}
}

void UIBattleUnit::OnMouseOver(EventData& data)
{
	if (!data.isDragging && lpBattleUnit)
	{
		ObserverManager::GetSingleton()->Notify("ViewInfo", lpBattleUnit->GetUnit());
		isHover = true;
	}
}

void UIBattleUnit::OnMouseOut(EventData& data)
{
	ObserverManager::GetSingleton()->Notify("HideInfo", this);
	isHover = false;
}

