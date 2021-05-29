#include "Card.h"
#include "Image.h"

void Card::Init()
{
	lpIconImg = nullptr;
	lpCardImg = nullptr;

	pos = { 0, 0 };

	width = 41 * 2;
	height = 58 * 2;

	SetRect(&rc, pos.x, pos.y, pos.x + width, pos.y + height);

	hFont = FontManager::GetSingleton()->GetFont("Uµ¸¿ò", 8);
}

void Card::Release()
{
	GameObject::Release();
}

void Card::Update(float deltaTime)
{
	SetRect(&rc, (int)pos.x, (int)pos.y, (int)pos.x + width, (int)pos.y + height);
}

void Card::Render(HDC hdc)
{
	SetTextAlign(hdc, TA_CENTER);
	SetBkMode(hdc, TRANSPARENT);
	if (lpCardImg)
	{
		lpCardImg->Render(hdc, rc.left, rc.top);
		hOldFont = (HFONT)SelectObject(hdc, hFont);
		ExtTextOut(hdc, rc.left + width / 2, rc.top + height / 2 + 27, ETO_CLIPPED, &rc, name.c_str(), name.length(), NULL);
		SelectObject(hdc, hOldFont);
	}
	SetBkMode(hdc, OPAQUE);
	SetTextAlign(hdc, TA_LEFT);
}

string Card::ToString()
{
	return name + "|" + desc;
}
