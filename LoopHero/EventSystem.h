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
	float deltaTime;
	EVENT_TYPE type;
	POINT point;
	vector<EventTrigger*> vlpTargets;
	// 이전호버리스트
	set<EventTrigger*> slpLastTargets;
	// 이전 이벤트 타겟
	EventTrigger* lpLastTarget;

	EventTrigger* lpDragTarget;
	POINT dragPoint;
	bool isDragging;

	void Use() { message[(int)type] = false; }
	void Reset() { message[(int)type] = true; }
	bool IsRemain() { return message.any(); }
	bool IsRemain(EVENT_TYPE type) { return message[(int)type]; }
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
	// 이벤트 데이터 초기화
	void InitEventData(EventData& data);
	// UI이벤트 대상 찾기
	GameUI* FindDispatcherUI(vector<EventTrigger*>& vTargets, GameUI* lpCurrUI, POINT point);
	// Object이벤트 대상 찾기
	GameObject* FindDispatcherObject(vector<EventTrigger*>& vTargets, GameObject* lpCurrObject, POINT point);
	// 이벤트 처리
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

