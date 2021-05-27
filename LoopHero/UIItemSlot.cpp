#include "UIItemSlot.h"
#include "EquipItem.h"
#include "UIHorizontalScroll.h"
#include "UISprite.h"
#include "Unit.h"
#include "Utill.h"
#include "Image.h"

void UIItemSlot::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, UNIT_SLOT slot, EQUIP_PARTS parts)
{
	GameUI::Init(anchor, pos, width, height);

	this->slot = slot;
	this->parts = parts;
	this->lpItem = nullptr;
	this->lpSprite = nullptr;

	isHover = false;
	lpHighlight = ImageManager::GetSingleton()->FindImage("selection_33");
}

void UIItemSlot::Update(float deltaTime)
{
	if (lpSprite)
	{
		POINTFLOAT newPos = lpSprite->GetPos();
		newPos.x += -lpSprite->GetPos().x * 10 * deltaTime;
		newPos.y += -lpSprite->GetPos().y * 10 * deltaTime;
		lpSprite->SetPos(newPos);

		frame += 10 * deltaTime;
		if (frame >= lpHighlight->GetTotalFrame()) frame = 0;
	}

	GameUI::Update(deltaTime);
}

void UIItemSlot::Render(HDC hdc)
{
	//RenderRectangle(hdc, rc);
	GameUI::Render(hdc);

	if (isHover)
	{
		lpHighlight->Render(hdc, GetCenter().x, GetCenter().y, (int)frame, IMAGE_ALIGN::CENTER);
	}
}

void UIItemSlot::AddChildUI(GameUI* lpChild)
{
	if (vChildUI.size() > 0) RemoveChildUI();

	GameUI::AddChildUI(lpChild);
}

void UIItemSlot::RemoveChildUI(int index)
{
	GameUI::RemoveChildUI(index);
}

void UIItemSlot::OnDrop(EventData& data)
{
	if (typeid(*data.lpDragTarget) == typeid(UISprite))
	{
		UISprite* lpGameUI = (UISprite*)data.lpDragTarget;
		if (lpGameUI->GetGameObject() && typeid(*lpGameUI->GetGameObject()) == typeid(EquipItem))
		{
			EquipItem* lpEquipItem = (EquipItem*)lpGameUI->GetGameObject();
			if (lpEquipItem->GetParts() == parts)
			{
				// Àåºñ ÀåÂø
				GameData::GetSingleton()->GetUnit()->UseEquipItem(slot, lpEquipItem);

				lpGameUI->SetAnchor(UI_ANCHOR::MIDDLE);

				lpItem = lpEquipItem;
				lpSprite = lpGameUI;
				AddChildUI(lpGameUI);
			}
		}
	}
}

void UIItemSlot::OnMouseEnter(EventData& data)
{
}

void UIItemSlot::OnMouseOver(EventData& data)
{
	if (!data.isDragging && lpSprite)
	{
		isHover = true;
	}
}

void UIItemSlot::OnMouseOut(EventData& data)
{
	isHover = false;
}
