#include "UIHorizontalScroll.h"
#include "Utill.h"

void UIHorizontalScroll::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, HSCROLL_ALIGN align, HS_ARGS_INSERT insert, int maxItems, int padding)
{
	GameUI::Init(anchor, pos, width, height);
	this->align = align;
	this->insert = insert;
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

	POINTFLOAT currPos, destPos = { 0.0f, 0.0f }, newPos;
	for (int i = 0; i < vChildUI.size(); ++i)
	{
		currPos = vChildUI[i]->GetPos();
		newPos = currPos;
		newPos.x += (destPos.x - currPos.x) * 10 * deltaTime;
		newPos.y += (destPos.y - currPos.y) * 10 * deltaTime;

		vChildUI[i]->SetPos(newPos);
		destPos.x += vChildUI[i]->GetWidth() + destPadding;
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
			if (insert == HS_ARGS_INSERT::AFTER) RemoveChildUI(0);
			else RemoveChildUI(vChildUI.size() - 1);
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

	totalItemWidth += lpChild->GetWidth();
	float diff = totalItemWidth - width;
	if (diff > 0)
	{
		destPadding = (width - totalItemWidth) / vChildUI.size();
		if (destPadding > padding) destPadding = padding;
	}

	GameUI::AddChildUI(lpChild);
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
			if (destPadding > padding) destPadding = padding;
		}

		OnRemoveChild(item);
	}
}
