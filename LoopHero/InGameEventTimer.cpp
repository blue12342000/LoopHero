#include "InGameEventTimer.h"
#include "UIProgressBar.h"
#include "Image.h"

void InGameEventTimer::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpBackground = ImageManager::GetSingleton()->FindImage("ingame_bosstimer_background");

	dailyTimer = 0.0f;
	bossTimer = 0.0f;

	maxDailyTimer = 10;
	maxBossTimer = 10;

	lpDailyTimer = GameUI::Create<UIProgressBar>(this);
	lpDailyTimer->Init(UI_ANCHOR::LEFT_TOP, { 17.0f * 2, 11.0f }, 200, 4, UI_BAR_TYPE::HORIZON, "", "battle_unit_statusbar_hp");
	lpDailyTimer->SetTrackingData(dailyTimer);
	lpDailyTimer->SetTrackingMaxData(maxDailyTimer);

	lpBossTimer = GameUI::Create<UIProgressBar>(this);
	lpBossTimer->Init(UI_ANCHOR::LEFT_TOP, { 17.0f * 2, 41.0f }, 200, 4, UI_BAR_TYPE::HORIZON, "", "battle_unit_statusbar_hp");
	lpBossTimer->SetTrackingData(bind(&InGameEventTimer::GetBossTimer, this));
	lpBossTimer->SetTrackingMaxData(maxBossTimer);

	CreateAnimController("event_timer");

	AddEventHandler("IncreaseBossTimer", bind(&InGameEventTimer::IncreaseBossTimer, this, placeholders::_1));
}

void InGameEventTimer::Update(float deltaTime)
{
	dailyTimer += deltaTime;
	if (dailyTimer > maxDailyTimer)
	{
		ObserverManager::GetSingleton()->Notify("daily_spawn_monster", this);
		ObserverManager::GetSingleton()->Notify("daily_near_spawn_monster", this);
		dailyTimer = 0;
	}

	GameUI::Update(deltaTime);
}

void InGameEventTimer::Render(HDC hdc)
{
	lpBackground->Render(hdc, rc.left, rc.top);

	GameUI::Render(hdc);
}

void InGameEventTimer::IncreaseBossTimer(ObserverHandler* lpCaller)
{
	bossTimer += 0.1f;
	if (bossTimer > maxBossTimer) bossTimer = maxBossTimer;
}
