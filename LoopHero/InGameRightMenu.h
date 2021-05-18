#pragma once
#include "GameUI.h"

class Image;
class UIHorizontalScroll;
class InGameRightMenu : public GameUI
{
private:
	Image* lpBackground;


	UIHorizontalScroll* lpHScroll;

	vector<string> desc;
public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
};

