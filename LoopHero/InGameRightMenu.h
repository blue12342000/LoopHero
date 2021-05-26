#pragma once
#include "GameUI.h"

class Image;
class UIGrid;
class UIHorizontalScroll;
class UIButton;
class UIInGameInfo;
class InGameRightMenu : public GameUI
{
private:
	Image* lpBackground;

	UIGrid* lpHeroEquip;
	UIHorizontalScroll* lpHScroll;
	UIButton* lpButton;
	UIInGameInfo* lpGameInfo;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
};

