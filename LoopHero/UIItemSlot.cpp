#include "UIItemSlot.h"
#include "EquipItem.h"
#include "UISprite.h"
#include "Utill.h"

void UIItemSlot::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, EQUIP_PARTS parts)
{
	GameUI::Init(anchor, pos, width, height);

	this->parts = parts;
	this->lpItem = nullptr;
	this->lpSprite = nullptr;
}

void UIItemSlot::Update(float deltaTime)
{
	if (lpSprite)
	{
		POINTFLOAT newPos = lpSprite->GetPos();
		newPos.x += -lpSprite->GetPos().x * 10 * deltaTime;
		newPos.y += -lpSprite->GetPos().y * 10 * deltaTime;
		lpSprite->SetPos(newPos);
	}
	GameUI::Update(deltaTime);
}

void UIItemSlot::Render(HDC hdc)
{
	//RenderRectangle(hdc, rc);
	GameUI::Render(hdc);
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
	if (typeid(*data.lpTarget) == typeid(UISprite))
	{
		UISprite* lpGameUI = (UISprite*)data.lpTarget;
		if (lpGameUI->GetGameObject() && typeid(*lpGameUI->GetGameObject()) == typeid(EquipItem))
		{
			EquipItem* lpEquipItem = (EquipItem*)lpGameUI->GetGameObject();

			if (lpEquipItem->GetParts() == parts)
			{
				lpGameUI->SetAnchor(UI_ANCHOR::MIDDLE);

				lpItem = lpEquipItem;
				lpSprite = lpGameUI;
				AddChildUI(lpGameUI);
			}
		}
	}
}

void UIItemSlot::OnMouseOver(EventData& data)
{
}
