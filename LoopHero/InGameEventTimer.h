#pragma once
#include "GameUI.h"

class UIProgressBar;
class InGameEventTimer : public GameUI
{
private:
	Image* lpBackground;
	float dailyTimer;
	float maxDailyTimer;
	float bossTimer;
	float maxBossTimer;

	UIProgressBar* lpDailyTimer;
	UIProgressBar* lpBossTimer;
	
public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc) override;

	inline float GetBossTimer() { return bossTimer; }

	void IncreaseBossTimer(ObserverHandler* lpCaller);
};

