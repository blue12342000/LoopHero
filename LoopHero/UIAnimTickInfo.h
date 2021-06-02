#pragma once
#include "GameUI.h"
class UIAnimTickInfo : public GameUI
{
private:
	Image* lpBackground;
	Image* lpTickArrow;

	AnimationUIController* lpTargetAnim;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Render(HDC hdc) override;

	void SetTargetAnim(AnimationUIController* lpTargetAnim);
};

