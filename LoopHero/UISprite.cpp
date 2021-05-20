#include "UISprite.h"
#include "ImageManager.h"
#include "Image.h"
#include "GameObject.h"
#include "Utill.h"

void UISprite::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpImage = ImageManager::GetSingleton()->AddImage("UI_SPRITE_" + to_string(width) + "_" + to_string(height) + "_" + to_string(((__int64)this)), width, height, true, RGB(0, 170, 255));
	lpImage->Fill();
}

void UISprite::Render(HDC hdc)
{
	if (lpObject)
	{
		lpImage->Fill();
		lpObject->Render(lpImage->GetMemDC());
	}
	lpImage->Render(hdc, rc.left, rc.top);
	//unsigned int rgb = ((unsigned int)this) * 10;
	//RenderRectangle(hdc, rc, RGB((rgb % 150), (rgb % 150), (rgb % 150)));

	GameUI::Render(hdc);
}
