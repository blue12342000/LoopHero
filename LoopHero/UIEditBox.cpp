#include "UIEditBox.h"
#include "Utill.h"

void UIEditBox::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, INPUT_TYPE type)
{
	GameUI::Init(anchor, pos, width, height);
	this->type = type;

	hTextBox = CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, rc.left, rc.top, width, height, g_hWnd, (HMENU)0, NULL, NULL);
	hTextDC = GetDC(hTextBox);
}

void UIEditBox::Release()
{
	ReleaseDC(hTextBox, hTextDC);
	DestroyWindow(hTextBox);
	hTextBox = NULL;
	hTextDC = NULL;
	GameUI::Release();
}

void UIEditBox::Update(float deltaTime)
{
	//if (hTextBox) SetWindowPos(hTextBox, NULL, rc.left, rc.top, width, height, SWP_NOMOVE | SWP_NOZORDER);
	GameUI::Update(deltaTime);
}

void UIEditBox::Render(HDC hdc)
{
	//RenderRectangle(hTextDC, RECT{0,0,width, height}, RGB(0, 170, 255));
	GameUI::Render(hdc);
	//SetWindowPos(hTextBox, g_hWnd, rc.left, rc.top, width, height, SWP_NOMOVE);
}
