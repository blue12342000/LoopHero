#pragma once
#include "GameUI.h"

class Image;
class InGameRightMenu : public GameUI
{
private:
	Image* lpBackground;

	vector<string> desc;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Render(HDC hdc) override;
};

