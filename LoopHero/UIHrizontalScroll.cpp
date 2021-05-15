#include "UIHrizontalScroll.h"

void UIHrizontalScroll::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, HSCROLL_ALIGN align, int maxItems)
{
	GameUI::Init(anchor, pos, width, height);
	this->align = align;
	this->maxItems = maxItems;
}

void UIHrizontalScroll::Update(float deltaTime)
{

}

void UIHrizontalScroll::Render(HDC hdc)
{
	for (int i = 0; i < vGameUIs.size(); ++i)
	{
		vGameUIs[i]->Render(hdc);
	}
}

GameUI* UIHrizontalScroll::AddChildUI(GameUI* lpChild)
{
	lpChild->SetAnchor(UI_ANCHOR::MIDDLE);
	GameUI::AddChildUI(lpChild);

	POINTFLOAT lastItemPos;
	switch (align)
	{
	case HSCROLL_ALIGN::LEFT:
		if (vGameUIs.empty()) lastItemPos = { 0.0f, height / 2.0f };
		else lastItemPos = vGameUIs.back()->GetPos();
		vGameUIs.push_back(lpChild);
		vGameUIs.back()->SetPos({ lastItemPos.x + vGameUIs.back()->GetWidth() / 2.0f, lastItemPos.y });
		break;
	case HSCROLL_ALIGN::RIGHT:
		if (vGameUIs.empty()) lastItemPos = { (float)width, height / 2.0f };
		else lastItemPos = vGameUIs.back()->GetPos();
		vGameUIs.push_back(lpChild);
		vGameUIs.back()->SetPos({ lastItemPos.x - vGameUIs.back()->GetWidth() / 2.0f, lastItemPos.y });
		break;
	}

	return lpChild;
}

