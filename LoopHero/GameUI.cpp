#include "GameUI.h"

void GameUI::VaildChilds()
{
	auto it = vChilds.cbegin();
	while (it != vChilds.cend())
	{
		if ((*it)->lpParent != this)
		{
			it = vChilds.erase(it);
		}
		else
		{
			++it;
		}
	}
}

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
	if (lpParent)
	{
		lpParent->RemoveChild(this);
		return;
	}
	else
	{
		rc = { 0, 0, 0, 0 };
		origin = { 0.0f, 0.0f };
		pos = { 0.0f, 0.0f };
		anchor = UI_ANCHOR::LEFT_TOP;
		width = 0;
		height = 0;

		while (!vChilds.empty())
		{
			vChilds.back()->lpParent = nullptr;
			vChilds.back()->Release();
			vChilds.pop_back();
		}
	}
	ClearEventHandler();
	ObserverManager::GetSingleton()->RemoveObserver(this);
	PoolingManager::GetSingleton()->AddClass(this);
}

void GameUI::Update(float deltaTime)
{
	if (isVisible)
	{
		for (int i = 0; i < vChilds.size(); ++i)
		{
			vChilds[i]->Update(deltaTime);
		}
	}
}

void GameUI::LateUpdate(float deltaTime)
{
	if (isVisible)
	{
		Refresh();
		for (int i = 0; i < vChilds.size(); ++i)
		{
			vChilds[i]->LateUpdate(deltaTime);
		}
	}
}

void GameUI::Render(HDC hdc)
{
	if (isVisible)
	{
		for (int i = 0; i < vChilds.size(); ++i)
		{
			if (vChilds[i]->isVisible) vChilds[i]->Render(hdc);
		}
	}
}

void GameUI::InsertChild(GameUI* lpChild, int index)
{
	lpChild->SetParernt(this);
	vChilds.insert(vChilds.begin() + index, lpChild);
}

void GameUI::AddChild(GameUI* lpChild)
{
	lpChild->SetParernt(this);
	vChilds.push_back(lpChild);
}

void GameUI::RemoveChild(int index)
{
	if (index > -1 && index < vChilds.size())
	{
		auto it = (vChilds.begin() + index);
		(*it)->lpParent = nullptr;
		(*it)->Release();
		vChilds.erase(it);
	}
}

void GameUI::RemoveChild(GameUI* lpChild)
{
	if (!lpChild) return;

	RemoveChild(find(vChilds.begin(), vChilds.end(), lpChild) - vChilds.begin());
}

void GameUI::SetWorldPos(POINT pos)
{
	SetWorldPos(POINTFLOAT{ (float)pos.x, (float)pos.y });
}

void GameUI::SetWorldPos(POINTFLOAT pos)
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
	case UI_ANCHOR::LEFT_TOP:
	case UI_ANCHOR::MIDDLE:
		thisPos.x = thisPos.x - inRect.left;
		thisPos.y = thisPos.y - inRect.top;
		break;
	case UI_ANCHOR::LEFT_MIDDLE:
		thisPos.x = thisPos.x - inRect.left;
		thisPos.y = thisPos.y - (inRect.top + inRect.bottom) / 2;
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
	Refresh();
}

void GameUI::SetParernt(GameUI* lpParent)
{
	POINTFLOAT worldPos = GetWorldPos();
	GameUI* lpPreParent = this->lpParent;
	this->lpParent = lpParent;
	if (lpPreParent)
	{
		lpPreParent->VaildChilds();
	}
	Refresh();
	SetWorldPos(worldPos);
}
