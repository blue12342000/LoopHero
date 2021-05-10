#pragma once
#include <Windows.h>

class GameUI
{
private:
	int width;
	int height;
	POINT pos;
	RECT hitBox;

public:
	virtual void Init() {}
	virtual void Release() {}
	virtual void Update() {}
	virtual void Render(HDC hdc) {}
};

