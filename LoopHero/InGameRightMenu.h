#pragma once
#include "GameUI.h"

class Image;
class UIHorizontalScroll;
class InGameRightMenu : public GameUI
{
private:
	Image* lpBackground;

	vector<string> desc;

	UIHorizontalScroll* lpHScroll[3];

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
};

