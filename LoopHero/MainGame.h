#pragma once
#include "LoopHero.h"
#include "Utill.h"

class Timer;
class MainGame
{
private:
	HDC hdc;
	HFONT hFont;
	Timer* timer;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render();

	LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

