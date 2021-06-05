#include "UITextField.h"
#include "Utill.h"

void UITextField::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);
	SetFont(UI_TEXT_HALIGN::LEFT, UI_TEXT_VALIGN::MIDDLE, UI_TEXT_LINE::MULTI, 14, RGB(255, 0, 0), "Bµ¸¿ò");
	this->style = UI_TEXT_STYLE::NORMAL;
}

void UITextField::Release()
{
	text = "";
	GameUI::Release();
}

void UITextField::Render(HDC hdc)
{
	//RenderRectangle(hdc, rc);
	SetBkMode(hdc, TRANSPARENT);
	COLORREF oldColor;
	if (hFont != NULL) hOldFont = (HFONT)SelectObject(hdc, hFont);
	if (style == UI_TEXT_STYLE::OUTLINE)
	{
		oldColor = SetTextColor(hdc, outLineColor);
		RECT outRc;
		for (int y = -1; y < 1; ++y)
		{
			for (int x = -1; x < 1; ++x)
			{
				if (x == y && x == 0) continue;
				outRc = { rc.left + x, rc.top + y, rc.right + x, rc.bottom + y };
				DrawText(hdc, text.c_str(), text.length(), &outRc, fontFormat);
			}
		}
		SetTextColor(hdc, oldColor);
	}

	oldColor = SetTextColor(hdc, color);
	DrawText(hdc, text.c_str(), text.length(), &rc, fontFormat);
	SetTextColor(hdc, oldColor);

	if (hFont != NULL) SelectObject(hdc, hOldFont);
	SetBkMode(hdc, OPAQUE);

	GameUI::Render(hdc);
}

void UITextField::SetFont(UI_TEXT_HALIGN hAlign, UI_TEXT_VALIGN vAlign, UI_TEXT_LINE line, int fontSize, COLORREF color, string fontName)
{
	this->hAlign = hAlign;
	this->vAlign = vAlign;
	this->line = line;
	this->fontSize = fontSize;
	this->color = color;
	this->fontName = fontName;

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
}

void UITextField::SetStyle(UI_TEXT_STYLE style, COLORREF outlineColor)
{
	this->style = style;
	this->outLineColor = outLineColor;
}
