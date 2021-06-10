#include "UIHierarchyItem.h"
#include "UIButton.h"
#include "AnimationUIController.h"
#include "PoolingManager.h"
#include "Text.h"
#include "Utill.h"

void UIHierarchyItem::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpText = PoolingManager::GetSingleton()->GetClass<Text>();
	lpText->Init("Bµ¸¿ò", 16, RGB(255, 255, 255));

	lpViewToggle = GameUI::Create<UIButton>(this);
	lpViewToggle->Init(UI_ANCHOR::RIGHT_TOP, { 4.0f, 3.0f }, 16, 13, UI_BUTTON_TYPE::TOGGLE);
	lpViewToggle->SetButtonImage("visible_16_13");
	isViewButton = true;
}

void UIHierarchyItem::Release()
{
	isViewButton = true;
	if (lpText)
	{
		lpText->Release();
		lpText = nullptr;
	}
	GameUI::Release();
}

void UIHierarchyItem::Render(HDC hdc)
{
	if (isHover) RenderRectangle(hdc, rc, RGB(20, 20, 20));
	if (lpText) lpText->Render(hdc, rc, TA_LEFT);

	GameUI::Render(hdc);
}

void UIHierarchyItem::SetTarget(GameUI* lpTarget, int depth)
{
	this->lpTarget = lpTarget;
	lpText->SetText(string((depth + 1) * 2, ' ') + "L " + typeid(*lpTarget).name());
	lpViewToggle->ClearFunc();
	lpViewToggle->PushBackFunc(bind(&GameUI::ToggleVisible, this->lpTarget));
	lpViewToggle->SetOn(!lpTarget->IsVisible());
}

void UIHierarchyItem::SetViewButton(bool isViewButton)
{
	this->isViewButton = isViewButton;
	lpViewToggle->SetVisible(isViewButton);
}

void UIHierarchyItem::OnClick(EventData& data)
{
	ObserverManager::GetSingleton()->Notify("OpenAnimController", lpTarget);
}

void UIHierarchyItem::OnMouseEnter(EventData& data)
{
	isHover = true;
}

void UIHierarchyItem::OnMouseOver(EventData& data)
{
	ObserverManager::GetSingleton()->Notify("hilight_hierarchy_item", this);
}

void UIHierarchyItem::OnMouseOut(EventData& data)
{
	isHover = false;
	ObserverManager::GetSingleton()->Notify("unhilight_hierarchy_item", this);
}
