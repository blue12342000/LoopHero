#include "GameUI.h"
#include <functional>

void GameUI::Refresh()
{
	RECT view;
	if (lpParent) view = lpParent->rc;
	else SetRect(&view, 0, 0, WINSIZE_WIDTH, WINSIZE_HEIGHT);

	switch (anchor)
	{
	case UI_ANCHOR::RIGHT_TOP:
		origin = { (float)view.right, (float)view.top };
		SetRect(&rc, origin.x - pos.x - width, origin.y + pos.y, origin.x - pos.x, origin.y + pos.y + height);
		break;
	case UI_ANCHOR::LEFT_BOTTOM:
		origin = { (float)view.left, (float)view.bottom };
		SetRect(&rc, origin.x + pos.x, origin.y - pos.y - height, origin.x + pos.x + width, origin.y - pos.y);
		break;
	case UI_ANCHOR::RIGHT_BOTTOM:
		origin = { (float)view.right, (float)view.bottom };
		SetRect(&rc, origin.x - pos.x - width, origin.y - pos.y - height, origin.x - pos.x, origin.y - pos.y);
		break;
	case UI_ANCHOR::MIDDLE:
		origin = { (view.right + view.left) / 2.0f, (view.top + view.bottom) / 2.0f };
		SetRect(&rc, origin.x + pos.x - width / 2, origin.y + pos.y - height / 2, origin.x + pos.x + width / 2, origin.y + pos.y + height / 2);
		break;
	case UI_ANCHOR::LEFT_MIDDLE:
		origin = { (float)view.left, (view.top + view.bottom) / 2.0f };
		SetRect(&rc, origin.x + pos.x, origin.y + pos.y - height / 2, origin.x + pos.x + width, origin.y + pos.y + height / 2);
		break;
	case UI_ANCHOR::RIGHT_MIDDLE:
		origin = { (float)view.right, (view.top + view.bottom) / 2.0f };
		SetRect(&rc, origin.x - pos.x - width, origin.y + pos.y - height / 2, origin.x - pos.x, origin.y + pos.y + height / 2);
		break;
	case UI_ANCHOR::TOP_MIDDLE:
		origin = { (view.right + view.left) / 2.0f, (float)view.top };
		SetRect(&rc, origin.x + pos.x - width / 2, origin.y + pos.y, origin.x + pos.x + width / 2, origin.y + pos.y + height);
		break;
	case UI_ANCHOR::BOTTOM_MIDDLE:
		origin = { (view.right + view.left) / 2.0f, (float)view.bottom };
		SetRect(&rc, origin.x + pos.x - width / 2, origin.y - pos.y - height, origin.x + pos.x + width / 2, origin.y - pos.y);
		break;
	case UI_ANCHOR::LEFT_TOP:
	default:
		origin = { (float)view.left, (float)view.top };
		SetRect(&rc, view.left + pos.x, view.top + pos.y, view.left + pos.x + width, view.top + pos.y + height);
		break;
	}
}

void GameUI::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	this->isVisible = true;
	this->anchor = anchor;
	this->pos = pos;
	this->width = width;
	this->height = height;

	Refresh();
}

void GameUI::Release()
{
	for (int i = 0; i < vChildUI.size(); ++i)
	{
		vChildUI[i]->Release();
		//delete vChildUI[i];
	}
	vChildUI.clear();

	PoolingManager::GetSingleton()->AddClass(this);
}

void GameUI::Update(float deltaTime)
{
	if (isVisible)
	{
		for (int i = 0; i < vChildUI.size(); ++i)
		{
			vChildUI[i]->Update(deltaTime);
		}
	}
}

void GameUI::LateUpdate(float deltaTime)
{
	if (isVisible)
	{
		Refresh();
		for (int i = 0; i < vChildUI.size(); ++i)
		{
			vChildUI[i]->LateUpdate(deltaTime);
		}
	}
}

void GameUI::Render(HDC hdc)
{
	if (isVisible)
	{
		for (int i = 0; i < vChildUI.size(); ++i)
		{
			if (vChildUI[i]->isVisible) vChildUI[i]->Render(hdc);
		}
	}
}

void GameUI::InsertChild(GameUI* lpChild, int index)
{
	lpChild->SetPos(lpChild->GetRealationPos(this));
	lpChild->lpParent = this;
	vChildUI.insert(vChildUI.begin() + index, lpChild);
}

void GameUI::AddChildUI(GameUI* lpChild)
{
	lpChild->SetParernt(this);
	vChildUI.push_back(lpChild);
}

void GameUI::RemoveChildUI(int index)
{
	if (index > -1 && index < vChildUI.size())
	{
		GameUI* lpGame = (*vChildUI.begin());
		//lpGame->Release();
		vChildUI.erase(vChildUI.begin() + index);
		//delete lpGame;
	}
}

void GameUI::RemoveChildUI(GameUI* lpChild)
{
	for (int i = 0; i < vChildUI.size(); ++i)
	{
		if (vChildUI[i] == lpChild)
		{
			RemoveChildUI(i);
			break;
		}
	}
}

void GameUI::SetWorldPos(POINT pos)
{
	POINTFLOAT thisPos = origin;

	switch (anchor)
	{
	case UI_ANCHOR::TOP_MIDDLE:
	case UI_ANCHOR::LEFT_MIDDLE:
	case UI_ANCHOR::LEFT_TOP:
	case UI_ANCHOR::MIDDLE:
		thisPos.x = pos.x - thisPos.x;
		thisPos.y = pos.y - thisPos.y;
		break;
	case UI_ANCHOR::RIGHT_MIDDLE:
	case UI_ANCHOR::RIGHT_TOP:
		thisPos.x = thisPos.x - pos.x;
		thisPos.y = pos.y - thisPos.y;
		break;
	case UI_ANCHOR::BOTTOM_MIDDLE:
	case UI_ANCHOR::LEFT_BOTTOM:
		thisPos.x = pos.x - thisPos.x;
		thisPos.y = thisPos.y - pos.y;
		break;
	case UI_ANCHOR::RIGHT_BOTTOM:
		thisPos.x = thisPos.x - pos.x;
		thisPos.y = thisPos.y - pos.y;
		break;
	}
	SetPos(thisPos);
}

inline POINTFLOAT GameUI::GetWorldPos()
{
	POINTFLOAT worldPos;
	switch (anchor)
	{
	case UI_ANCHOR::LEFT_TOP:
		worldPos = { (float)rc.left, (float)rc.top };
		break;
	case UI_ANCHOR::RIGHT_TOP:
		worldPos = { (float)rc.right, (float)rc.top };
		break;
	case UI_ANCHOR::LEFT_BOTTOM:
		worldPos = { (float)rc.left, (float)rc.bottom };
		break;
	case UI_ANCHOR::RIGHT_BOTTOM:
		worldPos = { (float)rc.right, (float)rc.bottom };
		break;
	case UI_ANCHOR::LEFT_MIDDLE:
		worldPos = { (float)rc.left, (rc.top + rc.bottom) * 0.5f };
		break;
	case UI_ANCHOR::RIGHT_MIDDLE:
		worldPos = { (float)rc.right, (rc.top + rc.bottom) * 0.5f };
		break;
	case UI_ANCHOR::TOP_MIDDLE:
		worldPos = { (rc.left + rc.right) * 0.5f, (float)rc.top };
		break;
	case UI_ANCHOR::BOTTOM_MIDDLE:
		worldPos = { (rc.left + rc.right) * 0.5f, (float)rc.bottom };
		break;
	case UI_ANCHOR::MIDDLE:
		worldPos = { (rc.left + rc.right) * 0.5f, (rc.top + rc.bottom) * 0.5f };
		break;
	default:
		worldPos = { 0.0f, 0.0f };
		break;
	}

	return worldPos;
}

POINTFLOAT GameUI::GetRealationPos(GameUI* lpOtherUI)
{
	POINTFLOAT thisPos = GetWorldPos();
	RECT inRect;
	if (lpOtherUI) inRect = lpOtherUI->GetRect();
	else inRect = { 0, 0, WINSIZE_WIDTH, WINSIZE_HEIGHT };

	switch (anchor)
	{
	case UI_ANCHOR::TOP_MIDDLE:
	case UI_ANCHOR::LEFT_MIDDLE:
	case UI_ANCHOR::LEFT_TOP:
	case UI_ANCHOR::MIDDLE:
		thisPos.x = thisPos.x - inRect.left;
		thisPos.y = thisPos.y - inRect.top;
		break;
	case UI_ANCHOR::RIGHT_MIDDLE:
	case UI_ANCHOR::RIGHT_TOP:
		thisPos.x = inRect.right - thisPos.x;
		thisPos.y = thisPos.y - inRect.top;
		break;
	case UI_ANCHOR::BOTTOM_MIDDLE:
	case UI_ANCHOR::LEFT_BOTTOM:
		thisPos.x = thisPos.x - inRect.left;
		thisPos.y = inRect.bottom - thisPos.y;
		break;
	case UI_ANCHOR::RIGHT_BOTTOM:
		thisPos.x = inRect.right - thisPos.x;
		thisPos.y = inRect.bottom - thisPos.y;
		break;
	}

	return thisPos;
}

void GameUI::SetAnchor(UI_ANCHOR anchor)
{
	if (this->anchor == anchor) return;

	this->anchor = anchor;
}

void GameUI::SetParernt(GameUI* lpParent)
{
	if (this->lpParent)
	{
		auto it = find(this->lpParent->vChildUI.begin(), this->lpParent->vChildUI.end(), this);
		this->lpParent->RemoveChildUI(it - this->lpParent->vChildUI.begin());
	}
	this->lpParent = lpParent;
	if (lpParent)
	{
		this->depth = lpParent->depth + 1;
	}
	else
	{
		this->depth = 0;
	}
	SetPos(GetRealationPos(lpParent));
}
