#pragma once
#include <Windows.h>
#include <vector>
#include <queue>
#include <functional>

using namespace std;

class GameUI;
class GameObject;
class EventTrigger;
struct EventData;
class EventSystem
{
private:
	EventData* lpPoint;
	EventData* lpBase;

	GameUI* lpGameUI;
	vector<GameObject*> vGameObjects;
	GameObject* lpGameObject;

	EventTrigger* lpFindTop;
	function<bool(GameUI*, GameUI*)> lpCompareUIFunc;
	priority_queue<GameUI*, vector<GameUI*>, function<bool(GameUI*, GameUI*)>> qlpGameUIByDepth;

	//GameObject* lpFindTop;
	//function<bool(GameUI*, GameUI*)> lpCompareFunc;
	//priority_queue<GameUI*, vector<GameUI*>, function<bool(GameUI*, GameUI*)>> qlpGameUIByDepth;

private:
	// 이벤트 데이터 초기화
	void InitEventData(EventData& data);
	// UI이벤트 대상 찾기
	EventTrigger* FindDispatcherUI(GameUI* lpCurrUI);

	EventTrigger* FindDispatcherObject(GameObject* lpCurrObject);

public:
	HRESULT Init();
	void Release();
	void Update(float deltaTime);
	void Render(HDC hdc);

	void SetGameUI(GameUI* lpGameUI) { this->lpGameUI = lpGameUI; }
	void AddGameObject(GameObject* lpObject) { vGameObjects.push_back(lpObject); }
};

