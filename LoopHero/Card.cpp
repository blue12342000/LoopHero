#include "Card.h"
#include "Image.h"

void Card::Init()
{
	lpIconImg = nullptr;
	lpCardImg = nullptr;

	pos = { 0, 0 };

	width = 41 * 2;
	height = 58 * 2;

	SetRect(&rc, pos.x - width / 2, pos.y - height / 2, pos.x + width / 2, pos.y + height / 2);

	hFont = CreateFont(13, 5, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, OUT_TT_ONLY_PRECIS, 0, DEFAULT_QUALITY, VARIABLE_PITCH | FF_ROMAN, TEXT("µ¸¿ò"));
}

void Card::Release()
{
	DeleteObject(hFont);
}

void Card::Update(float deltaTime)
{
	SetRect(&rc, (int)pos.x - width / 2, (int)pos.y - height / 2, (int)pos.x + width / 2, (int)pos.y + height / 2);
}

void Card::Render(HDC hdc)
{
	SetTextAlign(hdc, TA_CENTER);
	if (lpCardImg)
	{
		lpCardImg->Render(hdc, rc.left, rc.top);
		hOldFont = (HFONT)SelectObject(hdc, hFont);
		ExtTextOut(hdc, pos.x, pos.y + 27, ETO_CLIPPED, &rc, name.c_str(), name.length(), NULL);
		SelectObject(hdc, hOldFont);
	}
	SetTextAlign(hdc, TA_LEFT);
}