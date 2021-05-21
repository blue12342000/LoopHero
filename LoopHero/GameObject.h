#pragma once
#include "LoopHero.h"
#include "EventTrigger.h"
#include "ObserverHandler.h"

class GameObject : public EventTrigger, public ObserverHandler
{
protected:
	bool isVisible;
	int depth;
	POINTFLOAT pos;
	RECT rc;

	GameObject* lpParent;
	vector<GameObject*> vChilds;

protected:
	GameObject() {}

public:
	virtual ~GameObject() {}

	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void LateUpdate(float deltaTime) {}
	virtual void Render(HDC hdc) = 0;

	template<typename T>
	static T* Create(GameObject* lpParent = nullptr)
	{
		T* lpGameObject = new T;
		lpGameObject->lpParent = lpParent;
		if (!lpParent)
		{
			lpGameObject->depth = 0;
		}
		else
		{
			lpGameObject->depth = lpParent->depth + 1;
			lpParent->vChilds.push_back(lpGameObject);
		}
		return lpGameObject;
	}

	void InsertChild(GameObject* lpChild, int index);
	virtual void AddChild(GameObject* lpChild);
	virtual void RemoveChild(int index = 0);
	virtual void RemoveChild(GameObject* lpChild) final;

	virtual void SetWorldPos(POINTFLOAT pos) final;
	virtual POINTFLOAT GetWorldPos() final;
	virtual POINTFLOAT GetRealationPos(GameObject* lpOther) final;
	virtual void SetParent(GameObject* lpParent) final;

	virtual inline void SetPos(POINTFLOAT pos) final { this->pos = pos; }
	virtual inline void SetVisible(bool isVisible) final { this->isVisible = isVisible; }
	virtual inline POINTFLOAT GetPos() final { return pos; }
	virtual inline RECT GetRect() final { return rc; }
	virtual inline int GetChildCount() final { return vChilds.size(); }
	virtual inline int GetDepth() final { return depth; }
	virtual inline vector<GameObject*> GetChilds() final { return vChilds; }
	virtual inline bool IsVisible() final { return isVisible; }
};

