#include "InGameRightMenu.h"
#include "Image.h"
#include "InGameEventTimer.h"

void InGameRightMenu::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpBackground = ImageManager::GetSingleton()->FindImage("ingame_info_background");

	GameUI* lpEvent = GameUI::CreateUI<InGameEventTimer>();
	lpEvent->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ WINSIZE_WIDTH - 121.0f * 2, 0.0f }, 121 * 2, 27 * 2);
	this->AddChildUI(lpEvent);
}

void InGameRightMenu::Render(HDC hdc)
{
	lpBackground->Render(hdc, rc.left, rc.top, 0);
	GameUI::Render(hdc);
}
