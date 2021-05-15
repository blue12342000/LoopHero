#pragma once
#include "GameUI.h"

enum class HSCROLL_ALIGN
{
	LEFT,
	RIGHT
};

class GameUI;
class UIHorizontalScroll : public GameUI
{
private:
	HSCROLL_ALIGN align;
	int maxItems;

	float padding;
	float destPadding;

	float totalItemWidth;

public:
	virtual ~UIHorizontalScroll() {}

	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, HSCROLL_ALIGN align = HSCROLL_ALIGN::LEFT, int maxItems = 0, int padding = 4);
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	virtual void AddChildUI(GameUI* lpChild) final;
	virtual void RemoveChildUI(int index = 0) final;
};

