#pragma once
#include "GameUI.h"

enum class UI_TEXT_HALIGN
{
	LEFT,
	CENTER,
	RIGHT
};

enum class UI_TEXT_VALIGN
{
	TOP,
	MIDDLE,
	BOTTOM
};

enum class UI_TEXT_LINE
{
	SINGLE,
	MULTI
};

enum class UI_TEXT_STYLE
{
	NORMAL,
	OUTLINE
};

class UITextField : public GameUI
{
private:
	UI_TEXT_HALIGN hAlign;
	UI_TEXT_VALIGN vAlign;
	UI_TEXT_LINE line;
	UI_TEXT_STYLE style;

	int fontSize;

	UINT fontFormat;
	HFONT hFont;
	HFONT hOldFont;
	COLORREF color;
	COLORREF outLineColor;

	string fontName;

	string text;

public:
	virtual ~UITextField() {}

	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Release() override;
	virtual void Render(HDC hdc) override;

	void SetFont(UI_TEXT_HALIGN hAlign, UI_TEXT_VALIGN vAlign, UI_TEXT_LINE line, int fontSize, COLORREF color, string fontName);
	void SetStyle(UI_TEXT_STYLE style, COLORREF outLineColor = RGB(0, 0, 0));

	inline void SetText(string text) { this->text = text; }
	inline void AppendText(string text) { this->text += text; }
	inline string GetText() { return text; }
};

