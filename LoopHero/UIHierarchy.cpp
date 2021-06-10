#include "UIHierarchy.h"
#include "UIHierarchyItem.h"
#include "UIProgressBar.h"
#include "UIHorizontalScroll.h"
#include "UIAnimInspector.h"
#include "UIEditMenu.h"
#include "UIDebug.h"
#include "ImageManager.h"
#include "Image.h"
#include "Utill.h"

void UIHierarchy::SetHierarchyItem(UIHierarchyItem* lpParentItem, GameUI* lpGameUI, int depth)
{
	++depth;
	vector<GameUI*> vItemChilds = move(lpGameUI->GetChilds());
	UIHierarchyItem* lpHirarchyItem;

	GameUI* lpCurrItem;
	for (const auto& lpItem : vItemChilds)
	{
		if (typeid(*lpItem) == typeid(UIHierarchy)
			|| typeid(*lpItem) == typeid(UIAnimInspector)
			|| typeid(*lpItem) == typeid(UIDebug)
			|| typeid(*lpItem) == typeid(UIEditMenu)) continue;

		lpHirarchyItem = GameUI::Create<UIHierarchyItem>(lpParentItem);
		lpHirarchyItem->Init(UI_ANCHOR::LEFT_TOP, { 0.0f, (float)lpParentItem->GetHeight() + 5 }, lpParentItem->GetWidth(), 20);
		lpHirarchyItem->SetTarget(lpItem, depth);

		lpCurrItem = lpParentItem;
		lpCurrItem->SetHeight(lpCurrItem->GetHeight() + 25);
		while (lpCurrItem = lpCurrItem->GetParent())
		{
			if (typeid(*lpCurrItem) == typeid(UIHierarchy)) break;
			lpCurrItem->SetHeight(lpCurrItem->GetHeight() + 25);
		}

		SetHierarchyItem(lpHirarchyItem, lpItem, depth);
	}
}


void UIHierarchy::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpBackground = ImageManager::GetSingleton()->FindImage("layer_background");

	AddEventHandler("hilight_hierarchy_item", bind(&UIHierarchy::HilightHierarchyItem, this, placeholders::_1));
	AddEventHandler("unhilight_hierarchy_item", bind(&UIHierarchy::UnHilightHierarchyItem, this, placeholders::_1));
}

void UIHierarchy::Release()
{
	GameUI::Release();
}

void UIHierarchy::Render(HDC hdc)
{
	if (lpBackground) lpBackground->LoopRender(hdc, POINT{ rc.left, rc.top }, width, height, 0);

	GameUI::Render(hdc);

	if (lpHilight)
	{
		RenderRectangle(hdc, lpHilight->GetTarget()->GetRect(), RGB(0, 255, 170), true);
	}
}

void UIHierarchy::SetCanvus(GameUI* lpCanvus)
{
	this->lpCanvus = lpCanvus;

	while (GetChildCount() > 0)
	{
		RemoveChild(0);
	}

	lpRootItem = GameUI::Create<UIHierarchyItem>(this);
	lpRootItem->Init(UI_ANCHOR::LEFT_TOP, { 20.0f, 35.0f }, width - 40, 20);
	lpRootItem->SetTarget(lpCanvus, 0);
	lpRootItem->SetViewButton(false);

	SetHierarchyItem(lpRootItem, lpCanvus);
}

void UIHierarchy::HilightHierarchyItem(ObserverHandler* lpCaller)
{
	if (lpCaller && typeid(*lpCaller) == typeid(UIHierarchyItem))
	{
		lpHilight = (UIHierarchyItem*)lpCaller;
	}
}

void UIHierarchy::UnHilightHierarchyItem(ObserverHandler* lpCaller)
{
	lpHilight = nullptr;
}
