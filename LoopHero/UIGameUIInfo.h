#pragma once
#include "GameUI.h"

class UIButton;
class UIProgressBar;
class UIGameUIInfo : public GameUI
{
private:
	Image* lpBackground;
	UIButton* lpRadioButton[(int)UI_ANCHOR::NONE];
	UIProgressBar* lpAxisXBar;
	UIProgressBar* lpAxisYBar;

	GameUI* lpTarget;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Render(HDC hdc) override;

	void OpenAnimController(ObserverHandler* lpCaller);
};

