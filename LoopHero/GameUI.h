#pragma once
#include "LoopHero.h"

enum class UI_ANCHOR
{
	LEFT_TOP,
	RIGHT_TOP,
	LEFT_BOTTOM,
	RIGHT_BOTTOM,
	MIDDLE,
	NONE
};

class GameUI
{
private:
	GameUI* lpParent;
	vector<GameUI*> vChildUI;
	function<bool(POINT)> onClick;

protected:
	UI_ANCHOR anchor;
	int width;
	int height;
	POINTFLOAT origin;
	POINTFLOAT pos;
	RECT rc;

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
			SetRect(&rc, view.right - pos.x - width, view.top + pos.y, view.right - pos.x, view.top + pos.y + height);
			break;
		case UI_ANCHOR::LEFT_BOTTOM:
			origin = { (float)view.left, (float)view.bottom };
			SetRect(&rc, view.left + pos.x, view.bottom - pos.y - height, view.left + pos.x + width, view.bottom - pos.y);
			break;
		case UI_ANCHOR::RIGHT_BOTTOM:
			origin = { (float)view.right, (float)view.bottom };
			SetRect(&rc, view.right - pos.x - width, view.bottom - pos.y - height, view.right - pos.x, view.bottom - pos.y);
			break;
		case UI_ANCHOR::MIDDLE:
			origin = { (view.right + view.left) / 2.0f, (view.top + view.bottom) / 2.0f };
			SetRect(&rc, (view.right + view.left) / 2 + pos.x - width / 2, (view.top + view.bottom) / 2 + pos.y - height / 2, (view.right + view.left) / 2 + pos.x + width / 2, (view.top + view.bottom) / 2 + pos.y + height / 2);
			break;
		case UI_ANCHOR::LEFT_TOP:
		default:
			origin = { (float)view.left, (float)view.top };
			SetRect(&rc, view.left + pos.x, view.top + pos.y, view.left + pos.x + width, view.top + pos.y + height);
			break;
		}
	}

public:
	virtual ~GameUI() {}

	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
	{
		this->anchor = anchor;
		this->pos = pos;
		this->width = width;
		this->height = height;

		Refresh();
	}
	virtual void Release()
	{
		for (auto lpChild : vChildUI)
		{
			lpChild->Release();
			delete lpChild;
		}
		vChildUI.clear();
	}
	virtual void Update(float deltaTime) {}
	virtual void Render(HDC hdc)
	{
		for (auto lpChild : vChildUI)
		{
			lpChild->Render(hdc);
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

	GameUI* AddChildUI(GameUI* lpChild)
	{
		lpChild->lpParent = this;
		lpChild->Refresh();
		vChildUI.push_back(lpChild);
		return lpChild;
	}
};