#include "UIAnimInspector.h"
#include "UIGameUIInfo.h"
#include "ImageManager.h"
#include "Image.h"

void UIAnimInspector::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpBackground = ImageManager::GetSingleton()->FindImage("layer_background");

	lpUIInfo = GameUI::Create<UIGameUIInfo>(this);
	lpUIInfo->Init(UI_ANCHOR::LEFT_BOTTOM, { -300.0f, 0.0f }, 300, 300);

	AddEventHandler("OpenAnimController", bind(&UIAnimInspector::OpenAnimController, this, placeholders::_1));
}

void UIAnimInspector::Render(HDC hdc)
{
	if (lpBackground) lpBackground->LoopRender(hdc, POINT{ rc.left, rc.top }, width, height, 0);

	GameUI::Render(hdc);
}

void UIAnimInspector::OpenAnimController(ObserverHandler* lpCaller)
{
	isVisible = !isVisible;

	if (isVisible)
	{
		lpTarget = dynamic_cast<GameUI*>(lpTarget);
		if (isVisible = (lpTarget != nullptr))
		{

		}
	}
}
