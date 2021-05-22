#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <vector>
#include <queue>
#include <functional>
#include <bitset>
#include <set>

using namespace std;

enum class EVENT_TYPE
{
	CLICK,
	BEGIN_DRAG,
	DRAG,
	END_DRAG,
	DROP,
	MOUSE_ENTER,
	MOUSE_OVER,
	MOUSE_OUT,
	NONE
};

class EventTrigger;
struct EventData
{
	bitset<(int)EVENT_TYPE::NONE> message;
	bitset<(int)EVENT_TYPE::NONE> isRemain;
	float deltaTime;
	EVENT_TYPE type;
	POINT point;
	vector<EventTrigger*> vlpTargets;
	// ����ȣ������Ʈ
	set<EventTrigger*> slpLastTargets;
	// ���� �̺�Ʈ Ÿ��
	EventTrigger* lpLastTarget;

	EventTrigger* lpDragTarget;
	POINT dragPoint;
	bool isDragging;

	void Use() { isRemain[(int)type] = false; }
	void Reset() { isRemain[(int)type] = true; }
	bool IsRemain() { return isRemain.any(); }
	bool IsRemain(EVENT_TYPE type) { return isRemain[(int)type]; }
};

class GameUI;
class GameObject;
class EventSystem
{
private:
	EventData eventData;

	GameUI* lpGameUI;
	vector<GameObject*> vGameObjects;
	GameObject* lpGameObject;

	vector<EventTrigger*> vFindUIs;
	vector<EventTrigger*> vFindObjects;
	EventTrigger* lpObjectFindTop;

private:
	// �̺�Ʈ ������ �ʱ�ȭ
	void InitEventData(EventData& data);
	// UI�̺�Ʈ ��� ã��
	GameUI* FindDispatcherUI(vector<EventTrigger*>& vTargets, GameUI* lpCurrUI, POINT point);
	// Object�̺�Ʈ ��� ã��
	GameObject* FindDispatcherObject(vector<EventTrigger*>& vTargets, GameObject* lpCurrObject, POINT point);
	// �̺�Ʈ ó��
	void EventProcess();

public:
	HRESULT Init();
	void Release();
	void Update(float deltaTime);
	void Render(HDC hdc);

	void SetGameUI(GameUI* lpGameUI) { this->lpGameUI = lpGameUI; }
	void AddGameObject(GameObject* lpObject) { vGameObjects.push_back(lpObject); }
	void SetGameObject(GameObject* lpObject) { lpGameObject = lpObject; }
};

