#include "UIHorizontalScroll.h"
#include "Utill.h"

void UIHorizontalScroll::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, HSCROLL_ALIGN align, HS_ARGS_INSERT insert, int maxItems, int margin)
{
	GameUI::Init(anchor, pos, width, height);
	this->align = align;
	this->insert = insert;
	this->maxItems = maxItems;

	this->margin = margin;
	this->maxMargin = margin;
	this->destMargin = margin;
	this->totalItemWidth = 0;

	this->multiLineType = HSCROLL_MULTILINE::ALIGN;
	this->cols = 0;

	auto compareWidth = [](GameUI* a, GameUI* b) { if (a->GetWidth() == b->GetWidth()) { return a > b; } else { return a->GetWidth() > b->GetWidth(); } };
	sWidthSort = decltype(sWidthSort)(compareWidth);
	auto compareHeight = [](GameUI* a, GameUI* b) { if (a->GetHeight() == b->GetHeight()) { return a > b; } else { return a->GetHeight() > b->GetHeight(); } };
	sHeightSort = decltype(sHeightSort)(compareHeight);
}

void UIHorizontalScroll::Update(float deltaTime)
{
	if (abs(margin - destMargin) > FLT_EPSILON)
	{
		// 마진값 당기는 애니메이션 진행
		margin += (destMargin - margin) * 10 * deltaTime;
		if (abs(margin - destMargin) < FLT_EPSILON) margin = destMargin;
	}

	POINTFLOAT currPos, destPos = { 0.0f, 0.0f }, newPos;
	for (int i = 0; i < vChildUI.size(); ++i)
	{
		currPos = vChildUI[i]->GetPos();
		newPos = currPos;
		newPos.x += (destPos.x - currPos.x) * 10 * deltaTime;
		if (abs(newPos.x - destPos.x) < 0.01f) newPos.x = destPos.x;
		newPos.y += (destPos.y - currPos.y) * 10 * deltaTime;
		if (abs(newPos.y - destPos.y) < 0.01f) newPos.y = destPos.y;

		vChildUI[i]->SetPos(newPos);
		//destPos.x += vChildUI[i]->GetWidth() + destMargin;
		if (multiLineType != HSCROLL_MULTILINE::ZIGZAG) destPos.x += (*sWidthSort.begin())->GetWidth() + destMargin;
		else
		{
			if (((i / cols) % 2) == 0) destPos.x += (*sWidthSort.begin())->GetWidth() + destMargin;
			else destPos.x -= ((*sWidthSort.begin())->GetWidth() + destMargin);
		}

		if (cols > 0 && ((i + 1) % cols) == 0)
		{
			if (multiLineType != HSCROLL_MULTILINE::ZIGZAG) destPos.x = 0;
			else
			{
				if ((((i + 1) / cols) % 2) == 0) destPos.x = 0;
				else destPos.x = width - (*sWidthSort.begin())->GetWidth();
			}
			destPos.y += (*sHeightSort.begin())->GetHeight() + maxMargin;
		}
	}

	GameUI::Update(deltaTime);
}

void UIHorizontalScroll::Render(HDC hdc)
{
	RenderRectangle(hdc, rc, RGB(200, 200, 200));
	GameUI::Render(hdc);
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
	sHeightSort.insert(lpChild);
	sWidthSort.insert(lpChild);

	switch (align)
	{
	case HSCROLL_ALIGN::LEFT:
		if (cols > 0) lpChild->SetAnchor(UI_ANCHOR::LEFT_TOP);
		else lpChild->SetAnchor(UI_ANCHOR::LEFT_MIDDLE);
		break;
	case HSCROLL_ALIGN::RIGHT:
		if (cols > 0) lpChild->SetAnchor(UI_ANCHOR::RIGHT_TOP);
		else lpChild->SetAnchor(UI_ANCHOR::RIGHT_MIDDLE);
		break;
	}

	//totalItemWidth += lpChild->GetWidth();
	if (cols > 0) { totalItemWidth = min(vChildUI.size() + 1, cols) * (*sWidthSort.begin())->GetWidth(); }
	else { totalItemWidth = (vChildUI.size() + 1) * (*sWidthSort.begin())->GetWidth(); }
	float diff = totalItemWidth - width;
	if (diff > 0)
	{
		if (cols > 0) destMargin = (width - totalItemWidth) / min(vChildUI.size(), cols - 1);
		else destMargin = (width - totalItemWidth) / vChildUI.size();
		if (destMargin > maxMargin) destMargin = maxMargin;
	}

	if (insert == HS_ARGS_INSERT::AFTER) GameUI::InsertChildId(lpChild, vChildUI.size());
	else GameUI::InsertChildId(lpChild, 0);
}

void UIHorizontalScroll::RemoveChildUI(int index)
{
	if (index > -1 && index < vChildUI.size())
	{
		GameUI* item = *(vChildUI.begin() + index);
		vChildUI.erase(vChildUI.begin() + index);
		sHeightSort.erase(item);
		sWidthSort.erase(item);

		if (cols > 0) { totalItemWidth = min(vChildUI.size(), cols - 1) * (*sWidthSort.begin())->GetWidth(); }
		else { totalItemWidth = (vChildUI.size()) * (*sWidthSort.begin())->GetWidth(); }

		if (vChildUI.size() > 1)
		{
			destMargin = (width - totalItemWidth) / min(vChildUI.size() - 1, cols - 1);
			if (destMargin > maxMargin) destMargin = maxMargin;
		}

		item->Release();
		delete item;
	}
}

void UIHorizontalScroll::SetMultiLineType(HSCROLL_MULTILINE multiLineType, int cols)
{
	this->multiLineType = multiLineType;
	this->cols = cols;
}
