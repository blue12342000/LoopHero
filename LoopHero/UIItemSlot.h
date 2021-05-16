#pragma once
#include "UISprite.h"
class UIItemSlot : public UISprite
{
private:


public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height);
	virtual void Render(HDC hdc) override;
};

