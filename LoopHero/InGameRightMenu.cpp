#include "InGameRightMenu.h"
#include "Image.h"
#include "InGameEventTimer.h"

void InGameRightMenu::Init(UI_ANCHOR anchor, UI_EVENT_CAPTURE eventCapture, POINT pos, int width, int height)
{
	lpBackground = ImageManager::GetSingleton()->FindImage("ingame_info_background");
	
	this->anchor = anchor;
	this->eventCapture = eventCapture;
	this->pos = pos;
	this->width = width;
	this->height = height;

	GameUI* lpEvent = GameUI::CreateUI<InGameEventTimer>();
	lpEvent->Init(UI_ANCHOR::LEFT_TOP, UI_EVENT_CAPTURE::PASS, POINT{ 0, 0 }, 121 * 2, 27 * 2);
	this->AddChildUI(lpEvent);

	Refresh();
}

void InGameRightMenu::Render(HDC hdc)
{
	lpBackground->Render(hdc, rc.left, rc.top, 0);
	GameUI::Render(hdc);
}
