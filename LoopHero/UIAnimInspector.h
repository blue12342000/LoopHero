#pragma once
#include "GameUI.h"

class Image;
class UIGameUIInfo;
class UIButton;
class UIAnimInspector : public GameUI
{
private:
	Image* lpBackground;

	UIGameUIInfo* lpUIInfo;
	UIButton* lpAddButton;

	GameUI* lpTarget;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height);
	virtual void Render(HDC hdc);

	void OpenAnimController(ObserverHandler* lpCaller);
};

