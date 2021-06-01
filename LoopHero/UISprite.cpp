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

void UISprite::Release()
{
	if (lpObject)
	{
		lpObject->Release();
		lpObject = nullptr;
	}
	if (lpTexture)
	{
		lpTexture = nullptr;
	}
	GameUI::Release();
}

void UISprite::Render(HDC hdc)
{
	if (lpObject)
	{
		lpImage->Fill();
		lpObject->Render(lpImage->GetMemDC());
	}
	else if (lpTexture)
	{
		RECT inRect = { 0, 0, width, height };
		lpTexture->ResizeRender(lpImage->GetMemDC(), inRect, 0);
	}

	lpImage->Render(hdc, rc.left, rc.top);
	//unsigned int rgb = ((unsigned int)this) * 10;
	//RenderRectangle(hdc, rc, RGB((rgb % 150), (rgb % 150), (rgb % 150)));

	GameUI::Render(hdc);
}

void UISprite::OnMouseOver(EventData& data)
{
	if (!data.isDragging && lpObject)
	{
		ObserverManager::GetSingleton()->Notify("ViewInfo", lpObject);
	}
}

void UISprite::OnMouseOut(EventData& data)
{
	ObserverManager::GetSingleton()->Notify("HideInfo", this);
}

