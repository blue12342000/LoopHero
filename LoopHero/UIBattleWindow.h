#pragma once
#include "GameUI.h"

class Image;
class Animation;
class BattleField;
class UIBattleUnit;
class UIBattleWindow : public GameUI
{
private:
	Image* lpBackground;
	Animation* lpTimerBackground;
	Animation* lpHourglass;

	BattleField* lpBattleField;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	void BattleStart(ObserverHandler* lpCaller);
	void BattleEnd(ObserverHandler* lpCaller);
};

