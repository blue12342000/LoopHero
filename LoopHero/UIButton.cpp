#include "UIButton.h"
#include "ImageManager.h"
#include "Image.h"
#include <functional>

void UIButton::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, UI_BUTTON_TYPE type)
{
	GameUI::Init(anchor, pos, width, height);
	this->type = type;
	this->state = UI_BUTTON_STATE::NORMAL;

	lpButtonImage = nullptr;

	ObserverManager::GetSingleton()->RegisterObserver(this);
	AddOEventHandler("__OnClick_" + to_string((__int64)(this)), bind(&UIButton::CallBack, this, placeholders::_1));
}

void UIButton::Render(HDC hdc)
{
	if (lpButtonImage) lpButtonImage->Render(hdc, rc.left, rc.top, { 0, (int)state });
	GameUI::Render(hdc);
}

void UIButton::SetButtonImage(string imageKey)
{
	lpButtonImage = ImageManager::GetSingleton()->FindImage(imageKey);
}

void UIButton::SetButtonImage(Image* lpImage)
{
	lpButtonImage = lpImage;
}

void UIButton::OnClick()
{
	for (int i = 0; i < vClickFuncs.size(); ++i)
	{
		vClickFuncs[i]();
	}
}

void UIButton::CallBack(ObserverHandler* lpCaller)
{
	OnClick();
}

void UIButton::PushBackFunc(function<void()> func)
{
	vClickFuncs.push_back(move(func));
}

void UIButton::PopFunc()
{
	vClickFuncs.pop_back();
}

void UIButton::OnClick(EventData& data)
{
	state = UI_BUTTON_STATE::PRESS;
	ObserverManager::GetSingleton()->Notify("__OnClick_" + to_string((__int64)(this)), nullptr);
}

void UIButton::OnMouseOver(EventData& data)
{
	state = UI_BUTTON_STATE::HILIGHT;
}

void UIButton::OnMouseOut(EventData& data)
{
	state = UI_BUTTON_STATE::NORMAL;
}
