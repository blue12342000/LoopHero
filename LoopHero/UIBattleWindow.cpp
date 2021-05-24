#include "UIBattleWindow.h"
#include "Utill.h"

void UIBattleWindow::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);
}

void UIBattleWindow::Release()
{
	GameUI::Release();
}

void UIBattleWindow::Update(float deltaTime)
{
	GameUI::Update(deltaTime);
}

void UIBattleWindow::Render(HDC hdc)
{
	RenderRectangle(hdc, rc, RGB(200, 200, 200));

	GameUI::Render(hdc);
}
