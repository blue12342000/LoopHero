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
	EventData* lpData;
	GameUI* lpGameUI;
	priority_queue<GameUI*, vector<GameUI*>, function<bool(GameUI*, GameUI*)>> qlpGameUIByDepth;

private:
	// �̺�Ʈ ������ �ʱ�ȭ
	void InitEventData();
	// UI�̺�Ʈ ��� ã��
	GameUI* FindDispatcherUI(GameUI* lpCurrUI);

public:
	HRESULT Init();
	void Release();
	void Update(float deltaTime);
	void Render(HDC hdc);

	void SetGameUI(GameUI* lpGameUI) { this->lpGameUI = lpGameUI; }
};
