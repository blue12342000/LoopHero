#pragma once
#include "GameUI.h"

class InGameEventTimer : public GameUI
{
private:
	Image* lpBackground;
	
public:
	virtual void Init(UI_ANCHOR anchor, UI_EVENT_CAPTURE eventCapture, POINT pos, int width, int height) override;
	virtual void Render(HDC hdc) override;
};

