#pragma once
#include "GameUI.h"

class UIHierarchyItem;
class UIProgressBar;
class UIHorizontalScroll;
class UIHierarchy : public GameUI
{
private:
	Image* lpBackground;

	UIProgressBar* lpScroll;
	UIHorizontalScroll* lpMenu;

	GameUI* lpCanvus;
	UIHierarchyItem* lpRootItem;

	UIHierarchyItem* lpHilight;

private:
	void SetHierarchyItem(UIHierarchyItem* lpParentItem, GameUI* lpGameUI, int depth = 0);

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Release() override;
	virtual void Render(HDC hdc) override;

	void SetCanvus(GameUI* lpCanvus);

	void HilightHierarchyItem(ObserverHandler* lpCaller);

	void OnMouseOut(EventData& data) override;
};

