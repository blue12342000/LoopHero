#pragma once
#include "GameUI.h"

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

class GameUI;
class UIHorizontalScroll : public GameUI
{
private:
	HSCROLL_ALIGN align;
	HS_ARGS_INSERT insert;
	int maxItems;

	float margin;
	float maxMargin;
	float destMargin;

	float totalItemWidth;

public:
	virtual ~UIHorizontalScroll() {}

	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, HSCROLL_ALIGN align = HSCROLL_ALIGN::LEFT, HS_ARGS_INSERT insert = HS_ARGS_INSERT::BEFORE, int maxItems = 0, int padding = 4);
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	virtual void AddChildUI(GameUI* lpChild) final;
	virtual void RemoveChildUI(int index = 0) final;
};

