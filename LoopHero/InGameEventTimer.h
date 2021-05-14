#pragma once
#include "GameUI.h"

class InGameEventTimer : public GameUI
{
private:
	Image* lpBackground;
	
public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Render(HDC hdc) override;
};

