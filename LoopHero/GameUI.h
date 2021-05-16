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

class GameUI;
class UIEventHandler
{
protected:
	function<bool(POINT)> onClick;
	function<void()> onHover;
	function<void(GameUI*)> onChildRemove;
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

	void Refresh();
	bool OnClick(POINT point) { return PtInRect(&rc, point); }
	void OnRemoveChild(GameUI* lpChild);

public:
	virtual ~GameUI() {}

	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height);
	virtual void Release();
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);

	template<typename T>
	static T* CreateUI(GameUI* lpParent = nullptr)
	{
		T* lpGameUI = new T;
		lpGameUI->lpParent = lpParent;
		lpGameUI->onClick = bind(&GameUI::OnClick, lpGameUI, placeholders::_1);
		lpGameUI->onChildRemove = nullptr;
		if (lpParent) lpParent->vChildUI.push_back(lpGameUI);
		return lpGameUI;
	}

	void InsertChildId(GameUI* lpChild, int index);
	virtual void AddChildUI(GameUI* lpChild);
	virtual void RemoveChildUI(int index = 0);

	virtual inline void SetOnChildRemove(function<void(GameUI*)>* func) { if(func) onChildRemove = move(*func); }

	virtual POINTFLOAT GetWorldPos() final;
	virtual POINTFLOAT GetRealationPos(GameUI* lpOtherUI) final;

	virtual inline void SetAnchor(UI_ANCHOR anchor) { this->anchor = anchor; }
	virtual inline void SetPos(POINTFLOAT pos) final { this->pos = pos; Refresh(); }
	virtual inline void SetVisible(bool isVisible) final { this->isVisible = isVisible; }
	virtual inline POINTFLOAT GetPos() final { return pos; }
	virtual inline RECT GetRect() final { return rc; }
	virtual inline int GetWidth() final { return width; }
	virtual inline int GetHeight() final { return height; }
	virtual inline int GetChildCount() final { return vChildUI.size(); }
};