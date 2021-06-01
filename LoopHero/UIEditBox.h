#pragma once
#include "GameUI.h"

enum class INPUT_TYPE
{
	TEXT,
	NUMARIC
};

class UIEditBox : public GameUI
{
private:
	INPUT_TYPE type;
	HWND hTextBox;
	HDC hTextDC;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, INPUT_TYPE type);
	virtual void Release();
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);
};

