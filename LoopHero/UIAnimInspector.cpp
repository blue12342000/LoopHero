#include "UIAnimInspector.h"
#include "UIGameUIInfo.h"
#include "UIProgressBar.h"
#include "UITextField.h"
#include "UIHorizontalScroll.h"
#include "UITimeTick.h"
#include "UIAnimTickInfo.h"
#include "UIButton.h"
#include "ImageManager.h"
#include "Image.h"
#include "AnimationUIController.h"

void UIAnimInspector::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);
	state = ANIM_INSPECTOR_STATE::STOP;
	lpBackground = ImageManager::GetSingleton()->FindImage("layer_background");

	lpTimeLabel = GameUI::Create<UITextField>(this);
	lpTimeLabel->Init(UI_ANCHOR::RIGHT_TOP, { 33.0f, 38.0f }, 80, 15);
	lpTimeLabel->SetFont(UI_TEXT_HALIGN::CENTER, UI_TEXT_VALIGN::MIDDLE, UI_TEXT_LINE::SINGLE, 12, RGB(255, 255, 255), "B돋움");

	lpAnimTickInfo = GameUI::Create<UIAnimTickInfo>(this);
	lpAnimTickInfo->Init(UI_ANCHOR::LEFT_BOTTOM, { 23.0f, 15.0f }, width - 145, 150);

	lpTimeBar = GameUI::Create<UIProgressBar>(this);
	lpTimeBar->Init(UI_ANCHOR::LEFT_TOP, { 30.0f, 30.0f }, width - 160, 30, UI_BAR_TYPE::RANGE, "time_axis_bar", "axis_bar_button");
	lpTimeBar->SetRange(0.0f, 10.0f);
	lpTimeBar->SetTick(0.01f);
	lpTimeBar->PushBackFunc(bind([](UITextField* lpTimeLabel, float var) { lpTimeLabel->SetText(to_string(var).substr(0, to_string(var).find('.') + 3) + "s"); }, lpTimeLabel, placeholders::_1));
	lpTimeLabel->SetText(to_string(lpTimeBar->GetVar()).substr(0, to_string(lpTimeBar->GetVar()).find('.') + 3) + "s");

	lpAnimTimeScroll = GameUI::Create<UIHorizontalScroll>(this);
	lpAnimTimeScroll->Init(UI_ANCHOR::LEFT_TOP, { 30.0f, 70.0f }, width - 160, 50, HSCROLL_ALIGN::LEFT, HS_ARGS_INSERT::BEFORE, 0, 0);
	lpAnimTimeScroll->SetHScrollControl(HSCROLL_ITEM_CONTROL::FIXED);
	lpAnimTimeScroll->SetHScrollEscape(HSCROLL_ITEM_ESCAPE::DRAW);
	for (int i = 0; i < lpTimeBar->GetTotalTick(); ++i)
	{
		UITimeTick* lpTimeTick = GameUI::Create<UITimeTick>();
		lpTimeTick->Init(UI_ANCHOR::MIDDLE, { 0.0f, 0.0f }, 30, 50);
		lpTimeTick->SetTick(i);
		lpAnimTimeScroll->AddChild(lpTimeTick);
	}
	lpAnimTimeScroll->SetScroll(lpTimeBar);

	lpUIInfo = GameUI::Create<UIGameUIInfo>(this);
	lpUIInfo->Init(UI_ANCHOR::LEFT_BOTTOM, { -300.0f, 0.0f }, 300, 300);

	lpSaveBtn = GameUI::Create<UIButton>(this);
	lpSaveBtn->Init(UI_ANCHOR::RIGHT_TOP, { 45.0f, 60.0f }, 54, 34, UI_BUTTON_TYPE::BUTTON);
	lpSaveBtn->SetButtonImage("save_button_27_17");

	lpPlayBtn = GameUI::Create<UIButton>(this);
	lpPlayBtn->Init(UI_ANCHOR::RIGHT_TOP, { 45.0f, 100.0f }, 54, 34, UI_BUTTON_TYPE::BUTTON);
	lpPlayBtn->SetButtonImage("play_button_27_17");

	lpPauseBtn = GameUI::Create<UIButton>(this);
	lpPauseBtn->Init(UI_ANCHOR::RIGHT_TOP, { 45.0f, 140.0f }, 54, 34, UI_BUTTON_TYPE::BUTTON);
	lpPauseBtn->SetButtonImage("pause_button_27_17");

	AddEventHandler("OpenAnimController", bind(&UIAnimInspector::OpenAnimController, this, placeholders::_1));
}

void UIAnimInspector::Update(float deltaTime)
{
	if (state == ANIM_INSPECTOR_STATE::PLAY)
	{
		lpTimeBar->SetVar(lpTargetAnim->GetElapsedTime());
		lpTimeLabel->SetText(to_string(lpTimeBar->GetVar()).substr(0, to_string(lpTimeBar->GetVar()).find('.') + 3) + "s");
		if (!lpTargetAnim->IsPlay())
		{
			state = ANIM_INSPECTOR_STATE::STOP;
		}
	}

	GameUI::Update(deltaTime);
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
		lpTarget = dynamic_cast<GameUI*>(lpCaller);
		if (isVisible = (lpTarget != nullptr))
		{
			state = ANIM_INSPECTOR_STATE::STOP;
			lpTargetAnim = lpTarget->GetAnimController();
			if (lpTargetAnim)
			{
				lpSaveBtn->ClearFunc();
				lpSaveBtn->PushBackFunc(bind([](AnimationUIController* lpTargetAnim, UIProgressBar* lpTimeBar) {lpTargetAnim->AddEventTick(lpTimeBar->GetVarTick()); }, lpTargetAnim, lpTimeBar));
				lpSaveBtn->PushBackFunc(bind([](UIHorizontalScroll* lpAnimTimeScroll, UIProgressBar* lpTimeBar) {((UITimeTick*)lpAnimTimeScroll->GetChild(lpTimeBar->GetVarTick()))->SetUseTick(true); }, lpAnimTimeScroll, lpTimeBar));

				lpPlayBtn->ClearFunc();
				lpPlayBtn->PushBackFunc(bind(&AnimationUIController::Play, lpTargetAnim));
				lpPlayBtn->PushBackFunc(bind(&UIAnimInspector::SetState, this, ANIM_INSPECTOR_STATE::PLAY));

				lpPauseBtn->ClearFunc();
				lpPauseBtn->PushBackFunc(bind(&AnimationUIController::Stop, lpTargetAnim));
				lpPauseBtn->PushBackFunc(bind(&UIAnimInspector::SetState, this, ANIM_INSPECTOR_STATE::STOP));

				// 이벤트 적용된 틱 가져와서 기본값 넣어주기
				for (int i = 0, n = lpTimeBar->GetTotalTick(); i < n; ++i)
				{
					((UITimeTick*)lpAnimTimeScroll->GetChild(i))->SetUseTick(lpTargetAnim->IsEventExist(i));
				}
			}
		}
	}
}
