#pragma once
#include "GameUI.h"

class UISprite;
class AnimationUIController;
class UILogo : public GameUI
{
private:
	UISprite* lpLogo;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Release() override;
	virtual void Render(HDC hdc) override;

	void OnClick(EventData& data);
};

