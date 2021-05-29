#pragma once
#include "GameNode.h"
#include <Windows.h>
#include <string>

using namespace std;

class Text : public GameNode
{
private:
	int fontSize;
	string fontName;
	HFONT hFont;
	COLORREF color;
	string text;

public:
	void Init(string fontName, int fontSize, COLORREF color = RGB(0, 0, 0));
	void Release();
	void Render(HDC hdc, int x, int y, UINT textAlign = TA_LEFT);
	void Render(HDC hdc, RECT inRect, UINT textAlign = TA_LEFT);

	void ChangeSize(int fontSize);
	inline void SetText(string text) { this->text = text; }
};

