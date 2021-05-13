#include "InGameEventTimer.h"
#include "Image.h"

void InGameEventTimer::Init(UI_ANCHOR anchor, UI_EVENT_CAPTURE eventCapture, POINT pos, int width, int height)
{
	lpBackground = ImageManager::GetSingleton()->FindImage("ingame_bosstimer_background");

	this->anchor = anchor;
	this->eventCapture = eventCapture;
	this->pos = pos;
	this->width = width;
	this->height = height;

	Refresh();
}

void InGameEventTimer::Render(HDC hdc)
{
	lpBackground->Render(hdc, rc.left, rc.top);
}
