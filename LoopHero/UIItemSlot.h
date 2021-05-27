#pragma once
#include "GameUI.h"

enum class EQUIP_PARTS;
class EquipItem;
class UISprite;
class Image;
class UIItemSlot : public GameUI
{
private:
	UNIT_SLOT slot;
	EQUIP_PARTS parts;
	EquipItem* lpItem;
	UISprite* lpSprite;

	bool isHover;
	float frame;
	Image* lpHighlight;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, UNIT_SLOT slot, EQUIP_PARTS parts);
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	virtual void AddChildUI(GameUI* lpChild) final;
	virtual void RemoveChildUI(int index = 0) final;

	virtual void OnDrop(EventData& data) override;
	virtual void OnMouseEnter(EventData& data) override;
	virtual void OnMouseOver(EventData& data) override;
	virtual void OnMouseOut(EventData& data) override;
};

