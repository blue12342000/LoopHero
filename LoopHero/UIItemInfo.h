#pragma once
#include "GameUI.h"

class Image;
class UITextField;
class UIItemInfo : public GameUI
{
private:
	Image* lpBackground;
	Image* lpTopDeco;
	Image* lpBottomDeco;

	UITextField* lpTag;
	UITextField* lpName;
	UITextField* lpContent;

	EquipItem* lpEquipItem;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	void SetEquipItem(EquipItem* lpEquipItem);
	void SetTag(string text);
};

