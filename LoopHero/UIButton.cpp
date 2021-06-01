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
	groupId = to_string((__int64)(this));

	AddEventHandler("__OnClick_" + to_string((__int64)(this)), bind(&UIButton::CallBack, this, placeholders::_1));
	if (type == UI_BUTTON_TYPE::RADIO) AddEventHandler("__OnClick_button_" + groupId, bind(&UIButton::RadioCallBack, this, placeholders::_1));
}

void UIButton::Release()
{
	vClickFuncs.clear();
	GameUI::Release();
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
	state = UI_BUTTON_STATE::NORMAL;
}

void UIButton::RadioCallBack(ObserverHandler* lpCaller)
{
	if (typeid(*lpCaller) == typeid(UIButton))
	{
		UIButton* lpButton = (UIButton*)lpCaller;
		if (lpButton == this)
		{
			state = UI_BUTTON_STATE::PRESS;
		}
		else
		{
			state = UI_BUTTON_STATE::NORMAL;
		}
	}
}

void UIButton::ClearFunc()
{
	vClickFuncs.clear();
}

void UIButton::PushBackFunc(function<void()> func)
{
	vClickFuncs.push_back(move(func));
}

void UIButton::PopFunc()
{
	vClickFuncs.pop_back();
}

void UIButton::SetRadioGroup(string groupId)
{
	if (this->groupId != groupId)
	{
		RemoveEventHandler("__OnClick_button_" + this->groupId);
		AddEventHandler("__OnClick_button_" + groupId, bind(&UIButton::RadioCallBack, this, placeholders::_1));
		this->groupId = groupId;
	}
}

void UIButton::OnClick(EventData& data)
{
	state = UI_BUTTON_STATE::PRESS;
	ObserverManager::GetSingleton()->Notify("__OnClick_" + to_string((__int64)(this)), this);
	if (type == UI_BUTTON_TYPE::RADIO)
	{
		ObserverManager::GetSingleton()->Notify("__OnClick_button_" + groupId, this);
	}
}

void UIButton::OnMouseOver(EventData& data)
{
	if (type == UI_BUTTON_TYPE::BUTTON)
	{
		state = UI_BUTTON_STATE::HILIGHT;
	}
	else
	{
		if (state == UI_BUTTON_STATE::NORMAL)
		{
			state = UI_BUTTON_STATE::HILIGHT;
		}
	}
}

void UIButton::OnMouseOut(EventData& data)
{
	if (type == UI_BUTTON_TYPE::BUTTON)
	{
		state = UI_BUTTON_STATE::NORMAL;
	}
	else
	{
		if (state == UI_BUTTON_STATE::HILIGHT)
		{
			state = UI_BUTTON_STATE::NORMAL;
		}
	}
}
