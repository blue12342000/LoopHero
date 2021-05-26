#include "UITextField.h"
#include "Utill.h"

void UITextField::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);
	SetFont(UI_TEXT_HALIGN::LEFT, UI_TEXT_VALIGN::MIDDLE, UI_TEXT_STYLE::NORMAL, UI_TEXT_LINE::MULTI, 14, RGB(255, 0, 0), "°íµñ");
}

void UITextField::Release()
{
	DeleteObject(hFont);
	hFont = NULL;
}

void UITextField::Render(HDC hdc)
{
	COLORREF oldColor = SetTextColor(hdc, color);
	hOldFont = (HFONT)SelectObject(hdc, hFont);
	DrawText(hdc, text.c_str(), text.length(), &rc, fontFormat);
	SelectObject(hdc, hOldFont);
	SetTextColor(hdc, oldColor);
}

void UITextField::SetFont(UI_TEXT_HALIGN hAlign, UI_TEXT_VALIGN vAlign, UI_TEXT_STYLE style, UI_TEXT_LINE line, int fontSize, COLORREF color, string fontName)
{
	if (hFont != NULL) DeleteObject(hFont);

	this->hAlign = hAlign;
	this->vAlign = vAlign;
	this->style = style;
	this->line = line;
	this->fontSize = fontSize;
	this->color = color;
	this->fontName = fontName;

	float size = 0.3527f * 5;
	fontSize = (int)(size * fontSize);
	if (fontSize < 8) fontSize = 8;

	UINT fontWidth, italic;
	switch (style)
	{
	case UI_TEXT_STYLE::BOLD_ITALIC:
		fontWidth = FW_BOLD;
		italic = 1;
		break;
	case UI_TEXT_STYLE::BOLD:
		fontWidth = FW_BOLD;
		italic = 0;
		break;
	case UI_TEXT_STYLE::NORMAL:
		fontWidth = FW_NORMAL;
		italic = 0;
		break;
	case UI_TEXT_STYLE::ITALIC:
	default:
		fontWidth = FW_NORMAL;
		italic = 1;
		break;
	}

	if (this->line == UI_TEXT_LINE::MULTI) this->fontFormat = DT_WORDBREAK;
	else this->fontFormat = DT_SINGLELINE;
	switch (hAlign)
	{
	case UI_TEXT_HALIGN::CENTER:
		this->fontFormat |= DT_CENTER;
		break;
	case UI_TEXT_HALIGN::RIGHT:
		this->fontFormat |= DT_RIGHT;
		break;
	case UI_TEXT_HALIGN::LEFT:
	default:
		this->fontFormat |= DT_LEFT;
		break;
	}

	switch (vAlign)
	{
	case UI_TEXT_VALIGN::MIDDLE:
		this->fontFormat |= DT_VCENTER;
		break;
	case UI_TEXT_VALIGN::BOTTOM:
		this->fontFormat |= DT_BOTTOM;
		break;
	case UI_TEXT_VALIGN::TOP:
		this->fontFormat |= DT_TOP;
	default:
		break;
	}

	hFont = CreateFont(fontSize, fontSize / 2, 0, 0, fontWidth, italic, 0, 0, HANGEUL_CHARSET, OUT_TT_ONLY_PRECIS, 0, DEFAULT_QUALITY, VARIABLE_PITCH | FF_ROMAN, fontName.c_str());
	/*
	In_ int cHeight, _In_ int cWidth, _In_ int cEscapement, _In_ int cOrientation, _In_ int cWeight, _In_ DWORD bItalic,
    _In_ DWORD bUnderline, _In_ DWORD bStrikeOut, _In_ DWORD iCharSet, _In_ DWORD iOutPrecision, _In_ DWORD iClipPrecision,
    _In_ DWORD iQuality, _In_ DWORD iPitchAndFamily, _In_opt_ LPCSTR pszFaceName
	*/
}
