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

enum class UI_EVENT_CAPTURE
{
	BLOCK,
	PASS,
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
	UI_EVENT_CAPTURE eventCapture;
	int width;
	int height;
	POINT pos;
	RECT rc;

protected:
	GameUI() {}

	virtual bool OnClick(POINT pos) { return false; }
	inline void Refresh()
	{
		RECT view;
		if (lpParent) view = lpParent->rc;
		else SetRect(&view, 0, 0, WINSIZE_WIDTH, WINSIZE_HEIGHT);

		switch (anchor)
		{
		case UI_ANCHOR::RIGHT_TOP:
			SetRect(&rc, view.right - pos.x - width, view.top + pos.y, view.right - pos.x, view.top + pos.y + height);
			break;
		case UI_ANCHOR::LEFT_BOTTOM:
			SetRect(&rc, view.left + pos.x, view.top - pos.y - height, view.left + pos.x + width, view.top - pos.y);
			break;
		case UI_ANCHOR::RIGHT_BOTTOM:
			SetRect(&rc, view.right - pos.x - width, view.top - pos.y - height, view.right - pos.x, view.top - pos.y);
			break;
		case UI_ANCHOR::MIDDLE:
			SetRect(&rc, (view.right + view.left) / 2 + pos.x - width / 2, (view.top + view.bottom) / 2 + pos.y - height / 2, (view.right + view.left) / 2 + pos.x + width / 2, (view.top + view.bottom) / 2 + pos.y + height / 2);
			break;
		case UI_ANCHOR::LEFT_TOP:
		default:
			SetRect(&rc, view.left + pos.x, view.top + pos.y, view.left + pos.x + width, view.top + pos.y + height);
			break;
		}
	}

public:
	virtual ~GameUI() {}

	virtual void Init(UI_ANCHOR anchor, UI_EVENT_CAPTURE eventCapture, POINT pos, int width, int height) {}
	virtual void Release()
	{
		for (auto lpChild : vChildUI)
		{
			lpChild->Release();
			delete lpChild;
		}
		vChildUI.clear();
	}
	virtual void Update(float deltaTime) final
	{
		// 이벤트 버블링 처리
		// child의 애들부터 확인해서 onclick발생시 block이 발생할때까지 처리
	}
	virtual void Render(HDC hdc)
	{
		for (auto lpChild : vChildUI)
		{
			lpChild->Render(hdc);
		}
	}

	template<typename T>
	static GameUI* CreateUI()
	{
		GameUI* lpGameUI = new T;
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