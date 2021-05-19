#pragma once
#include "GameUI.h"

class UISprite;
class UIItemSlot : public GameUI
{
private:
	struct ItemSlot
	{
		POINTFLOAT pos;
		UISprite* lpSprite;
	};

private:
	ItemSlot slot;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height);
	virtual void Render(HDC hdc) override;
};

