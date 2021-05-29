#include "UITextField.h"
#include "Utill.h"

void UITextField::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);
	SetFont(UI_TEXT_HALIGN::LEFT, UI_TEXT_VALIGN::MIDDLE, UI_TEXT_LINE::MULTI, 14, RGB(255, 0, 0), "Bµ¸¿ò");
}

void UITextField::Release()
{
	if (hFont != NULL)
	{
		DeleteObject(hFont);
		hFont = NULL;
	}
}

void UITextField::Render(HDC hdc)
{
	COLORREF oldColor = SetTextColor(hdc, color);
	if (hFont != NULL) hOldFont = (HFONT)SelectObject(hdc, hFont);
	DrawText(hdc, text.c_str(), text.length(), &rc, fontFormat);
	if (hFont != NULL) SelectObject(hdc, hOldFont);
	SetTextColor(hdc, oldColor);
}

void UITextField::SetFont(UI_TEXT_HALIGN hAlign, UI_TEXT_VALIGN vAlign, UI_TEXT_LINE line, int fontSize, COLORREF color, string fontName)
{
	this->hAlign = hAlign;
	this->vAlign = vAlign;
	this->line = line;
	this->fontSize = fontSize;
	this->color = color;
	this->fontName = fontName;

	float size = 0.3527f * 5;
	fontSize = (int)(size * fontSize);
	if (fontSize < 8) fontSize = 8;

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

	hFont = FontManager::GetSingleton()->GetFont(fontName, fontSize);

	/*
	In_ int cHeight, _In_ int cWidth, _In_ int cEscapement, _In_ int cOrientation, _In_ int cWeight, _In_ DWORD bItalic,
    _In_ DWORD bUnderline, _In_ DWORD bStrikeOut, _In_ DWORD iCharSet, _In_ DWORD iOutPrecision, _In_ DWORD iClipPrecision,
    _In_ DWORD iQuality, _In_ DWORD iPitchAndFamily, _In_opt_ LPCSTR pszFaceName
	*/
}
