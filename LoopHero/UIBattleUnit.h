#pragma once
#include "GameUI.h"

class Image;
class Unit;
class UIProgressBar;
class UISprite;
class BattleUnit;
class UIBattleUnit : public GameUI
{
private:
	UISprite* lpSprite;
	Image* lpHudBack;
	UIProgressBar* lpHpBar;
	UIProgressBar* lpActionBar;

	bool isHover;
	Image* lpHover;
	BattleUnit* lpBattleUnit;

	float action;
	float maxAction;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	void SetBattleUnit(BattleUnit* lpBattleUnit);

	void OnMouseOver(EventData& data);
	void OnMouseOut(EventData& data);
};

