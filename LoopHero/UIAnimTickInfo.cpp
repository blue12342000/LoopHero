#include "UIAnimTickInfo.h"
#include "ImageManager.h"
#include "Image.h"

void UIAnimTickInfo::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpBackground = ImageManager::GetSingleton()->FindImage("layer_background");
	lpTickArrow = ImageManager::GetSingleton()->FindImage("time_tick_arrow");
}

void UIAnimTickInfo::Render(HDC hdc)
{
	lpBackground->LoopRender(hdc, POINT{ rc.left, rc.top }, width, height, 0);
	lpTickArrow->Render(hdc, (rc.left + rc.right) / 2 + 8, rc.top, 0);

	GameUI::Render(hdc);
}

void UIAnimTickInfo::SetTargetAnim(AnimationUIController* lpTargetAnim)
{

}
