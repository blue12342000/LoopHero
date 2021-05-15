#pragma once
#include "LoopHero.h"

enum class UI_ANCHOR
{
	LEFT_TOP,
	RIGHT_TOP,
	LEFT_BOTTOM,
	RIGHT_BOTTOM,
	LEFT_MIDDLE,
	RIGHT_MIDDLE,
	TOP_MIDDLE,
	BOTTOM_MIDDLE,
	MIDDLE,
	NONE
};

class UIEventHandler
{
protected:
	function<bool(POINT)> onClick;
	function<void()> onChildRemove;
};

class GameUI : private UIEventHandler
{
protected:
	bool isVisible;
	UI_ANCHOR anchor;
	int width;
	int height;
	POINTFLOAT origin;
	POINTFLOAT pos;
	RECT rc;

	GameUI* lpParent;
	vector<GameUI*> vChildUI;

protected:
	GameUI() {}

	virtual bool OnClick(POINT point) { return PtInRect(&rc, point); }
	inline void Refresh()
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
			SetRect(&rc, origin.x + pos.x - width / 2, origin.y + pos.y - height, origin.x + pos.x + width / 2, origin.y + pos.y);
			break;
		case UI_ANCHOR::LEFT_TOP:
		default:
			origin = { (float)view.left, (float)view.top };
			SetRect(&rc, view.left + pos.x, view.top + pos.y, view.left + pos.x + width, view.top + pos.y + height);
			break;
		}

		for (int i = 0; i < vChildUI.size(); ++i)
		{
			vChildUI[i]->Refresh();
		}
	}

public:
	virtual ~GameUI() {}

	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
	{
		this->isVisible = true;
		this->anchor = anchor;
		this->pos = pos;
		this->width = width;
		this->height = height;

		Refresh();
	}
	virtual void Release()
	{
		for (int i = 0; i < vChildUI.size(); ++i)
		{
			vChildUI[i]->Release();
			delete vChildUI[i];
		}
		vChildUI.clear();
	}
	virtual void Update(float deltaTime)
	{
		if (isVisible)
		{
			for (int i = 0; i < vChildUI.size(); ++i)
			{
				vChildUI[i]->Update(deltaTime);
			}
		}
	}
	virtual void Render(HDC hdc)
	{
		if (isVisible)
		{
			for (int i = 0; i < vChildUI.size(); ++i)
			{
				if (vChildUI[i]->isVisible) vChildUI[i]->Render(hdc);
			}
		}
	}

	template<typename T>
	static T* CreateUI()
	{
		T* lpGameUI = new T;
		lpGameUI->lpParent = nullptr;
		lpGameUI->onClick = bind(&GameUI::OnClick, lpGameUI, placeholders::_1);
		return lpGameUI;
	}

	virtual void AddChildUI(GameUI* lpChild)
	{
		lpChild->lpParent = this;
		lpChild->Refresh();
		vChildUI.push_back(lpChild);
	}
	virtual void RemoveChildUI(int index = 0)
	{
		if (index > -1 && index < vChildUI.size())
		{
			vChildUI.erase(vChildUI.begin() + index);
		}
	}

	virtual inline void SetOnChildRemove(function<void()>& func) { onChildRemove = move(func); }

	virtual inline void SetAnchor(UI_ANCHOR anchor) { this->anchor = anchor; }
	virtual inline void SetPos(POINTFLOAT pos) final { this->pos = pos; Refresh(); }
	virtual inline void SetVisible(bool isVisible) final { this->isVisible = isVisible; }
	virtual inline POINTFLOAT GetPos() final { return pos; }
	virtual inline RECT GetRect() final { return rc; }
	virtual inline int GetWidth() final { return width; }
	virtual inline int GetHeight() final { return height; }
};