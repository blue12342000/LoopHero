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

	lpSelected = nullptr;
	selectedIndex = -1;
	dragNextIndex = -1;
}

void UIHorizontalScroll::Update(float deltaTime)
{
	POINTFLOAT currPos, newPos;
	for (int i = 0, idx = 0; i < vChildUI.size(); ++i, ++idx)
	{
		if (i == dragNextIndex)
		{
			--idx;
			continue;
		}
		if (lpSelected == vChildUI[idx]) ++idx;

		currPos = vChildUI[idx]->GetPos();
		newPos = currPos;
		newPos.x += (vItemSlots[i].pos.x - currPos.x) * 10 * deltaTime;
		if (abs(newPos.x - vItemSlots[i].pos.x) < 0.01f) newPos.x = vItemSlots[i].pos.x;
		newPos.y += (vItemSlots[i].pos.y - currPos.y) * 10 * deltaTime;
		if (abs(newPos.y - vItemSlots[i].pos.y) < 0.01f) newPos.y = vItemSlots[i].pos.y;

		vChildUI[idx]->SetPos(newPos);
	}

	GameUI::Update(deltaTime);
}

void UIHorizontalScroll::Render(HDC hdc)
{
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

	if (insert == HS_ARGS_INSERT::AFTER) GameUI::InsertChildId(lpChild, vChildUI.size());
	else GameUI::InsertChildId(lpChild, 0);

	SlotResize();
}

void UIHorizontalScroll::RemoveChildUI(int index)
{
	if (index > -1 && index < vChildUI.size())
	{
		GameUI* item = *(vChildUI.begin() + index);
		vChildUI.erase(vChildUI.begin() + index);
		sHeightSort.erase(item);
		sWidthSort.erase(item);

		SlotResize();
	}
}

void UIHorizontalScroll::SetMultiLineType(HSCROLL_MULTILINE multiLineType, int cols)
{
	this->multiLineType = multiLineType;
	this->cols = cols;
}


void UIHorizontalScroll::OnClick(EventData& data)
{
	lpSelected = nullptr;
	RECT childRc;
	for (int i = vChildUI.size() - 1; i >= 0; --i)
	{
		childRc = vChildUI[i]->GetRect();
		if (PtInRect(&childRc, data.point))
		{
			lpSelected = vChildUI[i];
			lpSelected->SetAnchor(UI_ANCHOR::MIDDLE);
			lpSelected->SetPos(lpSelected->GetRealationPos(this));
			selectedIndex = i;
			dragNextIndex = i;
			break;
		}
	}
}

void UIHorizontalScroll::OnDrag(EventData& data)
{
	if (lpSelected)
	{
		lpSelected->SetWorldPos(data.point);

		int swapChildPos = -1;
		for (int i = 0; i < vItemSlots.size(); ++i)
		{
			if (PtInRect(&vItemSlots[i].rc, data.point))
			{
				swapChildPos = i;
				break;
			}
		}
		if (swapChildPos > -1) dragNextIndex = swapChildPos;
		else dragNextIndex = selectedIndex;
	}
}

void UIHorizontalScroll::OnEndDrag(EventData& data)
{
	if (lpSelected)
	{
		switch (align)
		{
		case HSCROLL_ALIGN::LEFT:
			if (cols > 0) lpSelected->SetAnchor(UI_ANCHOR::LEFT_TOP);
			else lpSelected->SetAnchor(UI_ANCHOR::LEFT_MIDDLE);
			break;
		case HSCROLL_ALIGN::RIGHT:
			if (cols > 0) lpSelected->SetAnchor(UI_ANCHOR::RIGHT_TOP);
			else lpSelected->SetAnchor(UI_ANCHOR::RIGHT_MIDDLE);
			break;
		}
		lpSelected->SetPos(lpSelected->GetRealationPos(this));

		if (dragNextIndex < selectedIndex)
		{
			// 앞으로
			vChildUI.erase(vChildUI.begin() + selectedIndex);
			vChildUI.insert(vChildUI.begin() + dragNextIndex, lpSelected);
		}
		else if (dragNextIndex > selectedIndex)
		{
			// 뒤로
			vChildUI.insert(vChildUI.begin() + (dragNextIndex + 1), lpSelected);
			vChildUI.erase(vChildUI.begin() + selectedIndex);
		}
		data.lpTarget = lpSelected;

		lpSelected = nullptr;
		selectedIndex = -1;
		dragNextIndex = -1;
	}
}

void UIHorizontalScroll::SlotResize()
{
	if (vChildUI.empty()) return;

	int slotWidth = (*sWidthSort.begin())->GetWidth();
	int slotHeight = (*sHeightSort.begin())->GetHeight();

	if (cols > 0) { totalItemWidth = min(vChildUI.size(), cols - 1) * (*sWidthSort.begin())->GetWidth(); }
	else { totalItemWidth = (vChildUI.size()) * (*sWidthSort.begin())->GetWidth(); }

	if (vChildUI.size() > 1)
	{
		margin = (width - totalItemWidth) / min(vChildUI.size() - 1, cols - 1);
		if (margin > maxMargin) margin = maxMargin;
	}

	ItemSlot slot;
	POINTFLOAT slotPos = { 0.0f, 0.0f };
	vItemSlots.resize(vChildUI.size());
	for (int i = 0; i < vChildUI.size(); ++i)
	{
		vItemSlots[i].pos = slotPos;
		switch (align)
		{
		case HSCROLL_ALIGN::LEFT:
			if (cols > 0) vItemSlots[i].worldPos = { rc.left + slotPos.x, rc.top + slotPos.y };
			else vItemSlots[i].worldPos = { rc.left + slotPos.x, (rc.top + rc.bottom) * 0.5f + slotPos.y };
			SetRect(&vItemSlots[i].rc, vItemSlots[i].worldPos.x, vItemSlots[i].worldPos.y, vItemSlots[i].worldPos.x + slotWidth, vItemSlots[i].worldPos.y + slotHeight);
			break;
		case HSCROLL_ALIGN::RIGHT:
			if (cols > 0) vItemSlots[i].worldPos = { rc.right - slotPos.x, rc.top + slotPos.y };
			else vItemSlots[i].worldPos = { rc.right - slotPos.x, (rc.top + rc.bottom) * 0.5f + slotPos.y };
			SetRect(&vItemSlots[i].rc, vItemSlots[i].worldPos.x - slotWidth, vItemSlots[i].worldPos.y, vItemSlots[i].worldPos.x, vItemSlots[i].worldPos.y + slotHeight);
			break;
		}

		if (multiLineType != HSCROLL_MULTILINE::ZIGZAG) slotPos.x += slotWidth + margin;
		else
		{
			if (((i / cols) % 2) == 0) slotPos.x += slotWidth + margin;
			else slotPos.x -= (slotWidth + margin);
		}

		if (cols > 0 && ((i + 1) % cols) == 0)
		{
			if (multiLineType != HSCROLL_MULTILINE::ZIGZAG) slotPos.x = 0;
			else
			{
				if ((((i + 1) / cols) % 2) == 0) slotPos.x = 0;
				else slotPos.x = width - slotWidth;
			}
			slotPos.y += slotHeight + maxMargin;
		}
	}
}
