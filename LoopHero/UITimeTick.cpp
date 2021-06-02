#include "UITimeTick.h"
#include "ImageManager.h"
#include "Image.h"
#include "Utill.h"

void UITimeTick::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpBackground = ImageManager::GetSingleton()->FindImage("");
	isUseTick = false;
}

void UITimeTick::Render(HDC hdc)
{
	if (isUseTick)
	{
		RenderRectangle(hdc, rc, RGB(200, 140, 0));
	}
	else
	{
		unsigned int rgb = ((unsigned int)this) * 10;
		RenderRectangle(hdc, rc, RGB((rgb % 256), (rgb % 256), (rgb % 256)));
	}
	GameUI::Render(hdc);
}

void UITimeTick::OnClick(EventData& data)
{
	isUseTick = !isUseTick;
}
