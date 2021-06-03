#pragma once
#include "LoopHero.h"

class Timer;
class MainGame
{
private:
	HDC hdc;
	Timer* timer;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render();

	LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

