#pragma once
#include "GameUI.h"

enum class EQUIP_PARTS;
class UISprite;
class UIItemSlot : public GameUI
{
private:
	EQUIP_PARTS parts;
	UISprite* lpItem;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, EQUIP_PARTS parts);
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	virtual void AddChildUI(GameUI* lpChild) final;
	virtual void RemoveChildUI(int index = 0) final;

	virtual void OnDrop(EventData& data) override;
};

