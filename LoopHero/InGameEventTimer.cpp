#include "InGameEventTimer.h"
#include "Image.h"

void InGameEventTimer::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpBackground = ImageManager::GetSingleton()->FindImage("ingame_bosstimer_background");
}

void InGameEventTimer::Render(HDC hdc)
{
	lpBackground->Render(hdc, rc.left, rc.top);
}
