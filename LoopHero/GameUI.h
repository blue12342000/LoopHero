#pragma once
#include "LoopHero.h"
#include "EventTrigger.h"
#include "ObserverHandler.h"
#include "GameNode.h"

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

class GameUI : public GameNode, public EventTrigger, public ObserverHandler
{
protected:
	bool isVisible;
	int depth;
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

protected:
	void Refresh();

public:
	virtual ~GameUI() {}

	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height);
	virtual void Release();
	virtual void Update(float deltaTime);
	virtual void LateUpdate(float deltaTime) final;
	virtual void Render(HDC hdc);

	template<typename T>
	static T* CreateUI(GameUI* lpParent = nullptr)
	{
		T* lpGameUI = PoolingManager::GetSingleton()->GetClass<T>();
		if (!lpGameUI) lpGameUI = new T;

		lpGameUI->lpParent = lpParent;
		if (lpParent) lpParent->vChildUI.push_back(lpGameUI);
		return lpGameUI;
	}

	void InsertChild(GameUI* lpChild, int index);
	virtual void AddChildUI(GameUI* lpChild);
	virtual void RemoveChildUI(int index);
	virtual void RemoveChildUI(GameUI* lpChild) final;

	virtual void SetWorldPos(POINT pos) final;
	virtual POINTFLOAT GetWorldPos() final;
	virtual POINTFLOAT GetRealationPos(GameUI* lpOtherUI) final;
	virtual void SetAnchor(UI_ANCHOR anchor) final;
	virtual void SetParernt(GameUI* lpParent) final;

	virtual inline void SetPos(POINTFLOAT pos) final { this->pos = pos; }
	virtual inline void SetVisible(bool isVisible) final { this->isVisible = isVisible; }
	virtual inline POINTFLOAT GetPos() final { return pos; }
	virtual inline RECT GetRect() final { return rc; }
	virtual inline int GetWidth() final { return width; }
	virtual inline int GetHeight() final { return height; }
	virtual inline int GetChildCount() final { return vChildUI.size(); }
	virtual inline int GetDepth() final { return depth; }
	virtual inline vector<GameUI*> GetChilds() final { return vChildUI; }
	virtual inline bool IsVisible() final { return isVisible; }
	virtual inline GameUI* GetParent() { return lpParent; }
	virtual inline POINT GetCenter() final { return { (rc.left + rc.right) / 2, (rc.top + rc.bottom) / 2 }; }
};