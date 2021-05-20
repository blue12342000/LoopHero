#include "UIItemSlot.h"
#include "EquipItem.h"
#include "UISprite.h"
#include "Utill.h"

void UIItemSlot::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, EQUIP_PARTS parts)
{
	GameUI::Init(anchor, pos, width, height);

	this->parts = parts;
	this->lpItem = nullptr;
}

void UIItemSlot::Update(float deltaTime)
{
	if (lpItem)
	{
		POINTFLOAT newPos = lpItem->GetPos();
		newPos.x += -lpItem->GetPos().x * 10 * deltaTime;
		newPos.y += -lpItem->GetPos().y * 10 * deltaTime;
		lpItem->SetPos(newPos);
	}
	GameUI::Update(deltaTime);
}

void UIItemSlot::Render(HDC hdc)
{
	RenderRectangle(hdc, rc);
	GameUI::Render(hdc);
}

void UIItemSlot::AddChildUI(GameUI* lpChild)
{
	if (vChildUI.size() > 0) RemoveChildUI();

	lpItem = (UISprite*)lpChild;
	GameUI::AddChildUI(lpChild);
}

void UIItemSlot::RemoveChildUI(int index)
{
	GameUI::RemoveChildUI(index);
}

void UIItemSlot::OnDrop(EventData& data)
{
	if (typeid(*data.lpTarget) == typeid(UISprite))
	{
		GameUI* lpGameUI = (GameUI*)data.lpTarget;
		lpGameUI->SetAnchor(UI_ANCHOR::MIDDLE);
		AddChildUI(lpGameUI);
	}
}
