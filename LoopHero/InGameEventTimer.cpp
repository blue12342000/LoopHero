#include "InGameEventTimer.h"
#include "UIProgressBar.h"
#include "Image.h"

void InGameEventTimer::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpBackground = ImageManager::GetSingleton()->FindImage("ingame_bosstimer_background");

	lpDailyTimer = GameUI::CreateUI<UIProgressBar>(this);
	lpDailyTimer->Init(UI_ANCHOR::LEFT_TOP, { 17.0f * 2, 7.0f * 2 }, 200, 4, UI_BAR_TYPE::HORIZON, "", "battle_unit_statusbar_hp");
	lpDailyTimer->SetTrackingData(&dailyTimer, 3);

	lpBossTimer = GameUI::CreateUI<UIProgressBar>(this);
	lpBossTimer->Init(UI_ANCHOR::LEFT_TOP, { 17.0f * 2, 22.0f * 2 }, 200, 4, UI_BAR_TYPE::HORIZON, "", "battle_unit_statusbar_hp");
	lpBossTimer->SetTrackingData(&bossTimer, 10);

	ObserverManager::GetSingleton()->RegisterObserver(this);
}

void InGameEventTimer::Update(float deltaTime)
{
	dailyTimer += deltaTime;
	if (dailyTimer > 3)
	{
		ObserverManager::GetSingleton()->Notify("DropCard", *this);
		dailyTimer = 0;
	}

	bossTimer += deltaTime;
	if (bossTimer > 10) bossTimer = 0;

	GameUI::Update(deltaTime);
}

void InGameEventTimer::Render(HDC hdc)
{
	lpBackground->Render(hdc, rc.left, rc.top);

	GameUI::Render(hdc);
}
