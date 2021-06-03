#pragma once
#include "LoopHero.h"
#include "EventTrigger.h"
#include "GameNode.h"

class GameObject : public GameNode, public EventTrigger
{
protected:
	bool isVisible;
	POINTFLOAT pos;
	RECT rc;

	GameObject* lpParent;
	vector<GameObject*> vChilds;

private:
	void VaildChilds();

public:
	virtual ~GameObject() {}

	virtual void Init();
	virtual void Release();
	virtual void Update(float deltaTime);
	virtual void LateUpdate(float deltaTime) {}
	virtual void Render(HDC hdc);

	template<typename T>
	static T* Create(GameObject* lpParent = nullptr)
	{
		T* lpGameObject = PoolingManager::GetSingleton()->GetClass<T>();
		lpGameObject->Init();
		lpGameObject->isVisible = true;
		lpGameObject->lpParent = lpParent;
		if (lpParent) lpParent->vChilds.push_back(lpGameObject);

		ObserverManager::GetSingleton()->RegisterObserver(lpGameObject);
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
	virtual inline GameObject* GetParent() final { return lpParent; }

	virtual inline void SetPos(POINTFLOAT pos) final { this->pos = pos; }
	virtual inline void SetVisible(bool isVisible) final { this->isVisible = isVisible; }
	virtual inline POINTFLOAT GetPos() final { return pos; }
	virtual inline RECT GetRect() final { return rc; }
	virtual inline int GetChildCount() final { return vChilds.size(); }
	virtual inline vector<GameObject*> GetChilds() final { return vChilds; }
	virtual inline bool IsVisible() final { return isVisible; }
};

