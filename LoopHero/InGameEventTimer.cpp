#include "InGameEventTimer.h"
#include "UIProgressBar.h"
#include "Image.h"

void InGameEventTimer::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpBackground = ImageManager::GetSingleton()->FindImage("ingame_bosstimer_background");

	maxDailyTimer = 3;
	maxBossTimer = 10;

	lpDailyTimer = GameUI::CreateUI<UIProgressBar>(this);
	lpDailyTimer->Init(UI_ANCHOR::LEFT_TOP, { 17.0f * 2, 7.0f * 2 }, 200, 4, UI_BAR_TYPE::HORIZON, "", "battle_unit_statusbar_hp");
	lpDailyTimer->SetTrackingData(dailyTimer);
	lpDailyTimer->SetTrackingMaxData(maxDailyTimer);

	lpBossTimer = GameUI::CreateUI<UIProgressBar>(this);
	lpBossTimer->Init(UI_ANCHOR::LEFT_TOP, { 17.0f * 2, 22.0f * 2 }, 200, 4, UI_BAR_TYPE::HORIZON, "", "battle_unit_statusbar_hp");
	lpBossTimer->SetTrackingData(bind(&InGameEventTimer::GetBossTimer, this));
	lpBossTimer->SetTrackingMaxData(maxBossTimer);

	ObserverManager::GetSingleton()->RegisterObserver(this);
}

void InGameEventTimer::Update(float deltaTime)
{
	dailyTimer += deltaTime;
	if (dailyTimer > maxDailyTimer)
	{
		
		ObserverManager::GetSingleton()->Notify("daily_spawn_monster", this);
		dailyTimer = 0;
	}

	bossTimer += deltaTime;
	if (bossTimer > maxBossTimer) bossTimer = 0;

	GameUI::Update(deltaTime);
}

void InGameEventTimer::Render(HDC hdc)
{
	lpBackground->Render(hdc, rc.left, rc.top);

	GameUI::Render(hdc);
}
