#pragma once
#include "GameUI.h"

class UISprite;
class UILogo : public GameUI
{
private:
	UISprite* lpLogo;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;

};

