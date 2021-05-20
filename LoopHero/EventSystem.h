#pragma once
#include <Windows.h>
#include <vector>
#include <queue>
#include <functional>

using namespace std;

class GameUI;
class EventTrigger;
struct EventData;
class EventSystem
{
private:
	EventData* lpPoint;
	EventData* lpBase;

	GameUI* lpGameUI;

	GameUI* lpFindTop;
	function<bool(GameUI*, GameUI*)> lpCompareFunc;
	priority_queue<GameUI*, vector<GameUI*>, function<bool(GameUI*, GameUI*)>> qlpGameUIByDepth;

private:
	// 이벤트 데이터 초기화
	void InitEventData(EventData& data);
	// UI이벤트 대상 찾기
	GameUI* FindDispatcherUI(GameUI* lpCurrUI);

public:
	HRESULT Init();
	void Release();
	void Update(float deltaTime);
	void Render(HDC hdc);

	void SetGameUI(GameUI* lpGameUI) { this->lpGameUI = lpGameUI; }
};

