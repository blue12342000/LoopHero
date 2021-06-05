#include "UIGameUIInfo.h"
#include "UIButton.h"
#include "UIProgressBar.h"
#include "ImageManager.h"
#include "Image.h"

void UIGameUIInfo::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);
	lpBackground = ImageManager::GetSingleton()->FindImage("layer_background");

	for (int i = 0, n = (int)UI_ANCHOR::NONE; i < n; ++i)
	{
		lpRadioButton[i] = GameUI::Create<UIButton>(this);
		lpRadioButton[i]->Init(UI_ANCHOR::MIDDLE, { -85.0f + (i % 3) * 85.0f, -100.0f + (i / 3) * 55.0f }, 80, 50, UI_BUTTON_TYPE::RADIO);
		lpRadioButton[i]->SetButtonImage("button_27_17");
		lpRadioButton[i]->SetRadioGroup("UI_ANCHOR");
	}

	lpAxisXBar = GameUI::Create<UIProgressBar>(this);
	lpAxisXBar->Init(UI_ANCHOR::BOTTOM_MIDDLE, { 0.0f, 50 }, 250, 15, UI_BAR_TYPE::RANGE, "axis_bar", "axis_bar_button");
	lpAxisXBar->SetRange(-300, WINSIZE_WIDTH + 300);
	lpAxisYBar = GameUI::Create<UIProgressBar>(this);
	lpAxisYBar->Init(UI_ANCHOR::BOTTOM_MIDDLE, { 0.0f, 30 }, 250, 15, UI_BAR_TYPE::RANGE, "axis_bar", "axis_bar_button");
	lpAxisYBar->SetRange(-300, WINSIZE_HEIGHT + 300);
}

void UIGameUIInfo::Render(HDC hdc)
{
	if (lpBackground) lpBackground->LoopRender(hdc, POINT{ rc.left, rc.top }, width, height, 0);
	GameUI::Render(hdc);
}

void UIGameUIInfo::RefreshBar()
{
	if (lpTarget)
	{
		POINTFLOAT worldPos = lpTarget->GetWorldPos();
		lpAxisXBar->SetVar(worldPos.x);
		lpAxisYBar->SetVar(worldPos.y);
	}
}

void UIGameUIInfo::OpenAnimController(GameUI* lpGameUI)
{
	if (lpGameUI)
	{
		lpTarget = lpGameUI;
		POINTFLOAT worldPos = lpTarget->GetWorldPos();
		for (int i = 0, n = (int)UI_ANCHOR::NONE; i < n; ++i)
		{
			lpRadioButton[i]->ClearFunc();
			lpRadioButton[i]->PushBackFunc(bind(&GameUI::SetAnchor, lpTarget, (UI_ANCHOR)i));
			lpRadioButton[i]->PushBackFunc(bind([](GameUI* lpTarget, UIProgressBar* lpAxisXBar) {lpAxisXBar->SetVar(lpTarget->GetWorldPos().x); }, lpTarget, lpAxisXBar));
			lpRadioButton[i]->PushBackFunc(bind([](GameUI* lpTarget, UIProgressBar* lpAxisYBar) {lpAxisYBar->SetVar(lpTarget->GetWorldPos().y); }, lpTarget, lpAxisYBar));
			if (lpTarget->GetAnchor() == (UI_ANCHOR)i)
			{
				lpRadioButton[i]->SetState(UI_BUTTON_STATE::PRESS);
			}
			else
			{
				lpRadioButton[i]->SetState(UI_BUTTON_STATE::NORMAL);
			}
		}
		lpAxisXBar->SetVar(worldPos.x);
		lpAxisYBar->SetVar(worldPos.y);

		lpAxisXBar->ClearFunc();
		lpAxisXBar->PushBackFunc(bind(&GameUI::SetWorldPosX, lpTarget, placeholders::_1));
		lpAxisYBar->ClearFunc();
		lpAxisYBar->PushBackFunc(bind(&GameUI::SetWorldPosY, lpTarget, placeholders::_1));
	}
}
