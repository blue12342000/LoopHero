#include "UIHorizontalScroll.h"
#include "UIProgressBar.h"
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

	control = HSCROLL_ITEM_CONTROL::FIXED;
	escape = HSCROLL_ITEM_ESCAPE::DRAW;

	lpSelected = nullptr;
	selectedIndex = -1;
	dragNextIndex = -1;
}

void UIHorizontalScroll::Release()
{
	lpScroll = nullptr;
	sHeightSort.clear();
	sWidthSort.clear();
	vItemSlots.clear();
	totalItemWidth = 0.0f;
	lpSelected = nullptr;
	GameUI::Release();
}

void UIHorizontalScroll::Update(float deltaTime)
{
	POINTFLOAT currPos, newPos;
	if (lpScroll == nullptr)
	{
		for (int i = 0, idx = 0; i < vChilds.size(); ++i, ++idx)
		{
			idx %= vChilds.size();
			if (lpSelected == vChilds[idx])
			{
				++idx;
				idx %= vChilds.size();
			}
			if (i == dragNextIndex)
			{
				--idx;
				continue;
			}
		
			currPos = vChilds[idx]->GetPos();
			newPos = currPos;
			newPos.x += (vItemSlots[i].pos.x - currPos.x) * 5.0f * deltaTime;
			if (abs(newPos.x - vItemSlots[i].pos.x) < 0.01f) newPos.x = vItemSlots[i].pos.x;
			newPos.y += (vItemSlots[i].pos.y - currPos.y) * 5.0f * deltaTime;
			if (abs(newPos.y - vItemSlots[i].pos.y) < 0.01f) newPos.y = vItemSlots[i].pos.y;
		
			vChilds[idx]->SetPos(newPos);
		}
		if (lpSelected && escape == HSCROLL_ITEM_ESCAPE::HIDE)
		{
			lpSelected->SetVisible(PtInRect(&rc, KeyManager::GetSingleton()->GetMousePoint()));
		}
	}
	else
	{
		for (const auto& lpGameUI : vChilds)
		{
			lpGameUI->SetVisible(false);
		}

		for (int i = lpScroll->GetVarTick() - viewItemCount / 2, n = lpScroll->GetVarTick() + viewItemCount / 2, idx = 0; i < n; ++i, ++idx)
		{
			if (i < 0 || i >= vChilds.size()) continue;

			vChilds[i]->SetPos(vItemSlots[idx].pos);
			vChilds[i]->SetVisible(true);
		}
	}

	GameUI::Update(deltaTime);
}

void UIHorizontalScroll::Render(HDC hdc)
{
	GameUI::Render(hdc);
}

void UIHorizontalScroll::AddChild(GameUI* lpChild)
{
	if (!lpChild) return;
	if (maxItems > 0 && vChilds.size() >= maxItems)
	{
		int diff = vChilds.size() - maxItems;
		for (int i = 0; i <= diff; ++i)
		{
			if (insert == HS_ARGS_INSERT::AFTER) RemoveChild(0);
			else RemoveChild(vChilds.size() - 1);
		}
	}

	if (lpSelected)
	{
		if (selectedIndex == dragNextIndex) ++dragNextIndex;
		++selectedIndex;
	}

	POINTFLOAT childWorldPos = lpChild->GetWorldPos();
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
	lpChild->SetWorldPos(childWorldPos);
	lpChild->Refresh();
	if (insert == HS_ARGS_INSERT::AFTER) GameUI::InsertChild(lpChild, vChilds.size());
	else GameUI::InsertChild(lpChild, 0);

	SlotResize();
}

void UIHorizontalScroll::RemoveChild(int index)
{
	if (index > -1 && index < vChilds.size())
	{
		if (index == selectedIndex)
		{
			selectedIndex = -1;
			dragNextIndex = -1;
			lpSelected = nullptr;
		}
		else if (lpSelected && index < selectedIndex)
		{
			--dragNextIndex;
			--selectedIndex;
			if (dragNextIndex < 0) dragNextIndex = selectedIndex;
			if (selectedIndex < 0) lpSelected = nullptr;
		}

		GameUI* item = *(vChilds.begin() + index);
		GameUI::RemoveChild(index);

		SlotResize();
	}
}

void UIHorizontalScroll::SetMultiLineType(HSCROLL_MULTILINE multiLineType, int cols)
{
	this->multiLineType = multiLineType;
	this->cols = cols;
}

void UIHorizontalScroll::SetScroll(UIProgressBar* lpScroll)
{
	this->lpScroll = lpScroll;
	
	SlotResize();
}

void UIHorizontalScroll::OnClick(EventData& data)
{
	if (control == HSCROLL_ITEM_CONTROL::FIXED)
	{
		data.Reset();
		return;
	}

	lpSelected = nullptr;
	RECT childRc;
	for (int i = vChilds.size() - 1; i >= 0; --i)
	{
		childRc = vChilds[i]->GetRect();
		if (PtInRect(&childRc, data.point))
		{
			lpSelected = vChilds[i];
			lpSelected->SetAnchor(UI_ANCHOR::MIDDLE);
			lpSelected->SetWorldPos(POINTFLOAT{ (childRc.left + childRc.right) / 2.0f, (childRc.top + childRc.bottom) / 2.0f });
			selectedIndex = i;
			dragNextIndex = i;
			break;
		}
	}
}

void UIHorizontalScroll::OnDrag(EventData& data)
{
	if (control == HSCROLL_ITEM_CONTROL::FIXED)
	{
		data.Reset();
		return;
	}

	if (lpSelected)
	{
		lpSelected->SetWorldPos(data.point);

		if (control == HSCROLL_ITEM_CONTROL::DRAG)
		{
			data.Reset();
			return;
		}

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
	if (control == HSCROLL_ITEM_CONTROL::FIXED)
	{
		data.Reset();
		return;
	}

	if (lpSelected)
	{
		RECT itemRc = lpSelected->GetRect();

		switch (align)
		{
		case HSCROLL_ALIGN::LEFT:
			if (cols > 0) lpSelected->SetAnchor(UI_ANCHOR::LEFT_TOP);
			else lpSelected->SetAnchor(UI_ANCHOR::LEFT_MIDDLE);
			lpSelected->SetWorldPos(POINTFLOAT{ (float)itemRc.left, (itemRc.top + itemRc.bottom) / 2.0f });
			break;
		case HSCROLL_ALIGN::RIGHT:
			if (cols > 0) lpSelected->SetAnchor(UI_ANCHOR::RIGHT_TOP);
			else lpSelected->SetAnchor(UI_ANCHOR::RIGHT_MIDDLE);
			lpSelected->SetWorldPos(POINTFLOAT{ (float)itemRc.right, (itemRc.top + itemRc.bottom) / 2.0f });
			break;
		}

		if (dragNextIndex < selectedIndex)
		{
			// 앞으로
			vChilds.erase(vChilds.begin() + selectedIndex);
			vChilds.insert(vChilds.begin() + dragNextIndex, lpSelected);
		}
		else if (dragNextIndex > selectedIndex)
		{
			// 뒤로
			vChilds.insert(vChilds.begin() + (dragNextIndex + 1), lpSelected);
			vChilds.erase(vChilds.begin() + selectedIndex);
		}
		data.lpDragTarget = lpSelected;
		lpSelected->SetVisible(true);
		lpSelected = nullptr;
		selectedIndex = -1;
		dragNextIndex = -1;

		SlotResize();
	}
}

void UIHorizontalScroll::SlotResize()
{
	sWidthSort.clear();
	sHeightSort.clear();
	if (vChilds.empty()) return;

	sWidthSort.insert(vChilds.begin(), vChilds.end());
	sHeightSort.insert(vChilds.begin(), vChilds.end());

	int slotWidth = (*sWidthSort.begin())->GetWidth();
	int slotHeight = (*sHeightSort.begin())->GetHeight();

	if (cols > 0) { totalItemWidth = min(vChilds.size(), cols - 1) * (*sWidthSort.begin())->GetWidth(); }
	else { totalItemWidth = (vChilds.size()) * (*sWidthSort.begin())->GetWidth(); }

	if (lpScroll == nullptr)
	{
		// 사이 거리값은 스크롤이 없을때만 연산해준다
		if (vChilds.size() > 1)
		{
			margin = (width - totalItemWidth) / min(vChilds.size() - 1, cols - 1);
			if (margin > maxMargin) margin = maxMargin;
		}
	}
	else
	{
		viewItemCount = width / slotWidth;
		margin = maxMargin;
	}

	ItemSlot slot;
	POINTFLOAT slotPos = { 0.0f, 0.0f };
	if (lpScroll == nullptr)
	{
		// 스크롤 없을때
		vItemSlots.resize(vChilds.size());
		for (int i = 0; i < vChilds.size(); ++i)
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
	else
	{
		// 스크롤 있을때
		int padding = (width - viewItemCount * (*sWidthSort.begin())->GetWidth()) / 2;
		slotPos.x = padding;
		vItemSlots.resize(viewItemCount);
		for (int i = 0; i < viewItemCount; ++i)
		{
			vItemSlots[i].pos = slotPos;
			switch (align)
			{
			case HSCROLL_ALIGN::LEFT:
				vItemSlots[i].worldPos = { rc.left + slotPos.x, (rc.top + rc.bottom) * 0.5f + slotPos.y };
				SetRect(&vItemSlots[i].rc, vItemSlots[i].worldPos.x, vItemSlots[i].worldPos.y, vItemSlots[i].worldPos.x + slotWidth, vItemSlots[i].worldPos.y + slotHeight);
				break;
			case HSCROLL_ALIGN::RIGHT:
				vItemSlots[i].worldPos = { rc.right - slotPos.x, (rc.top + rc.bottom) * 0.5f + slotPos.y };
				SetRect(&vItemSlots[i].rc, vItemSlots[i].worldPos.x - slotWidth, vItemSlots[i].worldPos.y, vItemSlots[i].worldPos.x, vItemSlots[i].worldPos.y + slotHeight);
				break;
			}
			slotPos.x += slotWidth + margin;
		}
	}
}
