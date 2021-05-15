#pragma once
#include "GameUI.h"

enum class HSCROLL_ALIGN
{
	LEFT,
	RIGHT
};

class GameUI;
class UIHrizontalScroll : public GameUI
{
private:
	HSCROLL_ALIGN align;
	int maxItems;

	vector<GameUI*> vGameUIs;

public:
	virtual ~UIHrizontalScroll() {}

	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, HSCROLL_ALIGN align = HSCROLL_ALIGN::LEFT, int maxItems = 5);
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	virtual GameUI* AddChildUI(GameUI* lpChild) final;
};

