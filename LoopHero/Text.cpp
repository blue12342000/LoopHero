#include "Text.h"
#include "PoolingManager.h"
#include "FontManager.h"

void Text::Init(string fontName, int fontSize, COLORREF color)
{
	this->fontName = fontName;
	this->fontSize = fontSize;
	this->color = color;

	hFont = FontManager::GetSingleton()->GetFont(fontName, fontSize);
}

void Text::Release()
{
	hFont = NULL;
	text = "";
	PoolingManager::GetSingleton()->AddClass(this);
}

void Text::Render(HDC hdc, int x, int y, UINT textAlign)
{
	SetBkMode(hdc, TRANSPARENT);
	COLORREF oldColor = SetTextColor(hdc, color);
	UINT oldTextAlign = SetTextAlign(hdc, textAlign);
	ExtTextOut(hdc, x, y, 0, nullptr, text.c_str(), text.length(), nullptr);
	SetTextAlign(hdc, oldTextAlign);
	SetTextColor(hdc, oldColor);
	SetBkMode(hdc, OPAQUE);
}

void Text::Render(HDC hdc, RECT inRect, UINT textAlign)
{
	SetBkMode(hdc, TRANSPARENT);
	COLORREF oldColor = SetTextColor(hdc, color);
	UINT oldTextAlign = SetTextAlign(hdc, textAlign);
	if (textAlign & TA_LEFT)
	{
		ExtTextOut(hdc, inRect.left, inRect.top, ETO_CLIPPED, &inRect, text.c_str(), text.length(), nullptr);
	}
	else if (textAlign & TA_CENTER)
	{
		ExtTextOut(hdc, (int)((inRect.left + inRect.right) * 0.5f), inRect.top, ETO_CLIPPED, &inRect, text.c_str(), text.length(), nullptr);
	}
	else if (textAlign & TA_RIGHT)
	{
		RECT drawRect = { 0,0,0,0 };
		DrawText(hdc, text.c_str(), text.length(), &drawRect, DT_CALCRECT);
		ExtTextOut(hdc, drawRect.left, drawRect.top, ETO_RTLREADING, &drawRect, text.c_str(), text.length(), nullptr);
	}
	else
	{
		ExtTextOut(hdc, inRect.left, inRect.top, ETO_CLIPPED, &inRect, text.c_str(), text.length(), nullptr);
	}
	SetTextColor(hdc, oldTextAlign);
	SetTextAlign(hdc, textAlign);
	SetBkMode(hdc, OPAQUE);
}

void Text::ChangeSize(int fontSize)
{
	this->fontSize = fontSize;
	hFont = FontManager::GetSingleton()->GetFont(fontName, fontSize);
}
