#include "UIAnimTickInfo.h"
#include "UITextField.h"
#include "ImageManager.h"
#include "Image.h"

void UIAnimTickInfo::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpBackground = ImageManager::GetSingleton()->FindImage("layer_background");
	lpTickArrow = ImageManager::GetSingleton()->FindImage("time_tick_arrow");

	lpTickLabel = GameUI::Create<UITextField>(this);
	lpTickLabel->Init(UI_ANCHOR::LEFT_TOP, { 25.0f, 25.0f }, 110, 20);
	lpTickLabel->SetFont(UI_TEXT_HALIGN::LEFT, UI_TEXT_VALIGN::MIDDLE, UI_TEXT_LINE::SINGLE, 16, RGB(255, 255, 255), "B³ª´®¹Ù¸¥°íµñ");
	lpTickLabel->SetText("TickPoint : ");

	lpTickPoint = GameUI::Create<UITextField>(this);
	lpTickPoint->Init(UI_ANCHOR::LEFT_TOP, { 135.0f, 25.0f }, 180, 20);
	lpTickPoint->SetFont(UI_TEXT_HALIGN::LEFT, UI_TEXT_VALIGN::MIDDLE, UI_TEXT_LINE::SINGLE, 16, RGB(255, 255, 255), "B³ª´®¹Ù¸¥°íµñ");
	lpTickPoint->SetText("-");

	lpTimeLabel = GameUI::Create<UITextField>(this);
	lpTimeLabel->Init(UI_ANCHOR::LEFT_TOP, { 25.0f, 55.0f }, 110, 20);
	lpTimeLabel->SetFont(UI_TEXT_HALIGN::LEFT, UI_TEXT_VALIGN::MIDDLE, UI_TEXT_LINE::SINGLE, 16, RGB(255, 255, 255), "B³ª´®¹Ù¸¥°íµñ");
	lpTimeLabel->SetText("ElapsedTime : ");

	lpElapsedTime = GameUI::Create<UITextField>(this);
	lpElapsedTime->Init(UI_ANCHOR::LEFT_TOP, { 135.0f, 55.0f }, 180, 20);
	lpElapsedTime->SetFont(UI_TEXT_HALIGN::LEFT, UI_TEXT_VALIGN::MIDDLE, UI_TEXT_LINE::SINGLE, 16, RGB(255, 255, 255), "B³ª´®¹Ù¸¥°íµñ");
	lpElapsedTime->SetText("-");
}

void UIAnimTickInfo::Render(HDC hdc)
{
	lpBackground->LoopRender(hdc, POINT{ rc.left, rc.top }, width, height, 0);
	lpTickArrow->Render(hdc, (rc.left + rc.right) / 2 + 8, rc.top, 0);

	GameUI::Render(hdc);
}

void UIAnimTickInfo::ViewAnimVariable(float time)
{
	int index = (int)(time / 0.01f + FLT_EPSILON);
	char text[256];
	if (index < vAnimVariables.size())
	{
		sprintf_s(text, "X : %.2f / Y : %.2f", vAnimVariables[index].position.x, vAnimVariables[index].position.y);
		lpTickPoint->SetText(text);
		sprintf_s(text, "%.2fs", vAnimVariables[index].elapsedTime);
		lpElapsedTime->SetText(text);
	}
	else if (!vAnimVariables.empty())
	{
		sprintf_s(text, "X : %.2f / Y : %.2f", vAnimVariables.back().position.x, vAnimVariables.back().position.y);
		lpTickPoint->SetText(text);
		sprintf_s(text, "%.2fs", vAnimVariables.back().elapsedTime);
		lpElapsedTime->SetText(text);
	}
	else
	{
		lpTickPoint->SetText("-");
		lpElapsedTime->SetText("-");
	}
}
