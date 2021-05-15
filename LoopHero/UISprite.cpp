#include "UISprite.h"
#include "ImageManager.h"
#include "Image.h"
#include "GameObject.h"

void UISprite::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpImage = ImageManager::GetSingleton()->AddImage("UI_SPRITE_" + to_string((__int64)this), width, height, true, RGB(0, 170, 255));
}

void UISprite::Render(HDC hdc)
{
	if (lpObject)
	{
		lpObject->Render(lpImage->GetMemDC());
	}
	lpImage->Render(hdc, rc.left, rc.top);
}
