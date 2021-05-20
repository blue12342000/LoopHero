#pragma once
#include "GameUI.h"
#include <set>

enum class HSCROLL_ALIGN
{
	LEFT,
	RIGHT
};

enum class HS_ARGS_INSERT
{
	BEFORE,
	AFTER
};

enum class HSCROLL_MULTILINE
{
	ALIGN,
	ZIGZAG
};


class GameUI;
class UIHorizontalScroll : public GameUI
{
protected:
	struct ItemSlot
	{
		POINTFLOAT pos;
		POINTFLOAT worldPos;
		RECT rc;
	};

protected:
	HSCROLL_ALIGN align;
	HS_ARGS_INSERT insert;
	HSCROLL_MULTILINE multiLineType;
	int maxItems;
	int cols;

	float margin;
	float maxMargin;
	float destMargin;

	float totalItemWidth;

	vector<ItemSlot> vItemSlots;
	set<GameUI*, function<bool(GameUI*, GameUI*)>> sHeightSort;
	set<GameUI*, function<bool(GameUI*, GameUI*)>> sWidthSort;

	GameUI* lpSelected;
	int selectedIndex;
	int dragNextIndex;

public:
	virtual ~UIHorizontalScroll() {}

	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, HSCROLL_ALIGN align = HSCROLL_ALIGN::LEFT, HS_ARGS_INSERT insert = HS_ARGS_INSERT::BEFORE, int maxItems = 0, int margin = 4);
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	virtual void AddChildUI(GameUI* lpChild) final;
	virtual void RemoveChildUI(int index = 0) final;
	
	bool ChildUIMaxHeight(GameUI* a, GameUI* b) {}
	void SetMultiLineType(HSCROLL_MULTILINE multiLineType, int cols);

	virtual void OnClick(EventData& data) override;
	virtual void OnDrag(EventData& data) override;
	virtual void OnEndDrag(EventData& data) override;

private:
	void SlotResize();
};

