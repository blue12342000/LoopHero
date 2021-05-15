#include "UIHorizontalScroll.h"
#include "Utill.h"

void UIHorizontalScroll::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, HSCROLL_ALIGN align, int maxItems, int padding)
{
	GameUI::Init(anchor, pos, width, height);
	this->align = align;
	this->maxItems = maxItems;

	this->padding = padding;
	this->destPadding = padding;
	this->totalItemWidth = 0;
}

void UIHorizontalScroll::Update(float deltaTime)
{
	if (abs(padding - destPadding) > FLT_EPSILON)
	{
		// 마진값 당기는 애니메이션 진행
		padding += (destPadding - padding) * 10 * deltaTime;
		if (abs(padding - destPadding) < FLT_EPSILON) padding = destPadding;
	}

	float diffX = 0, destX = 0;
	POINTFLOAT currPos;
	for (int i = 0; i < vChildUI.size(); ++i)
	{
		if (i > 0) destX += (float)vChildUI[i - 1]->GetWidth();
		currPos = vChildUI[i]->GetPos();
		diffX = destX - currPos.x;
		currPos.x += diffX * 10 * deltaTime;
		vChildUI[i]->SetPos(currPos);
		destX += destPadding;
	}
}

void UIHorizontalScroll::Render(HDC hdc)
{
	RenderRectangle(hdc, rc, RGB(200, 200, 200));
	for (int i = 0; i < vChildUI.size(); ++i)
	{
		vChildUI[i]->Render(hdc);
	}
}

void UIHorizontalScroll::AddChildUI(GameUI* lpChild)
{
	if (!lpChild) return;
	if (maxItems > 0 && vChildUI.size() >= maxItems)
	{
		int diff = vChildUI.size() - maxItems;
		for (int i = 0; i <= diff; ++i)
		{
			RemoveChildUI(0);
		}
	}

	switch (align)
	{
	case HSCROLL_ALIGN::LEFT:
		lpChild->SetAnchor(UI_ANCHOR::LEFT_MIDDLE);
		break;
	case HSCROLL_ALIGN::RIGHT:
		lpChild->SetAnchor(UI_ANCHOR::RIGHT_MIDDLE);
		break;
	}

	POINTFLOAT lastItemPos;
	if (vChildUI.empty()) lastItemPos = { 0.0f, 0.0f };
	else
	{
		lastItemPos = vChildUI.back()->GetPos();
		lastItemPos.x += vChildUI.back()->GetWidth();
		lastItemPos.x += padding;
	}
	GameUI::AddChildUI(lpChild);

	totalItemWidth += lpChild->GetWidth();
	float diff = lastItemPos.x + lpChild->GetWidth() - width;
	if (diff > 0)
	{
		lastItemPos.x -= diff;
		destPadding = (width - totalItemWidth) / (vChildUI.size() - 1);
		if (destPadding > 10) destPadding = 10;
	}
	lpChild->SetPos(lastItemPos);
}

void UIHorizontalScroll::RemoveChildUI(int index)
{
	if (index > -1 && index < vChildUI.size())
	{
		GameUI* item = *(vChildUI.begin() + index);
		totalItemWidth -= item->GetWidth();
		vChildUI.erase(vChildUI.begin() + index);

		if (vChildUI.size() > 1)
		{
			destPadding = (width - totalItemWidth) / (vChildUI.size() - 1);
			if (destPadding > 10) destPadding = 10;
		}

		item->Release();
		delete item;
	}
}
