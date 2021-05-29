#include "UIProgressBar.h"
#include "LoopHero.h"
#include "Image.h"

void UIProgressBar::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, UI_BAR_TYPE type, string back, string bar)
{
	GameUI::Init(anchor, pos, width, height);

	this->type = type;
	this->target = UI_BAR_TARGET::VARIABLE;
	this->maxTarget = UI_BAR_TARGET::VARIABLE;
	this->lpTargetVar = nullptr;
	this->lpMaxVar = nullptr;

	lpBackground = ImageManager::GetSingleton()->FindImage(back);
	lpBar = ImageManager::GetSingleton()->FindImage(bar);
}

void UIProgressBar::Release()
{
	lpTargetFunc = nullptr;
	lpMaxFunc = nullptr;
	GameUI::Release();
}

void UIProgressBar::Update(float deltaTime)
{
	GameUI::Update(deltaTime);
}

void UIProgressBar::Render(HDC hdc)
{
	if (lpBackground)
	{
		switch (type)
		{
		case UI_BAR_TYPE::HORIZON:
			lpBackground->PatternRender(hdc, rc.left, rc.top, width, height);
			break;
		case UI_BAR_TYPE::VERTICAL:
			lpBackground->PatternRender(hdc, rc.left, rc.bottom, width, height);
			break;
		}
	}

	if (lpBar)
	{
		float targetVar, maxVar;
		switch (target)
		{
		case UI_BAR_TARGET::FUNC:
			if (lpTargetFunc) targetVar = lpTargetFunc();
			else targetVar = 1;
			break;
		case UI_BAR_TARGET::VARIABLE:
		default:
			if (lpTargetVar) targetVar = *lpTargetVar;
			else targetVar = 1;
			break;
		}
		switch (maxTarget)
		{
		case UI_BAR_TARGET::FUNC:
			if (lpMaxFunc) maxVar = lpMaxFunc();
			else maxVar = 1;
			break;
		case UI_BAR_TARGET::VARIABLE:
		default:
			if (lpMaxVar) maxVar = *lpMaxVar;
			else maxVar = 1;
			break;
		}

		float ratio = targetVar / maxVar;
		if (ratio > 1) ratio = 1;

		switch (type)
		{
		case UI_BAR_TYPE::HORIZON:
			lpBar->PatternRender(hdc, rc.left, rc.top, (int)(width * ratio), height);
			break;
		case UI_BAR_TYPE::VERTICAL:
			lpBar->PatternRender(hdc, rc.left, rc.bottom, width, (int)(height * ratio));
			break;
		}
	}

	GameUI::Render(hdc);
}

void UIProgressBar::SetTrackingData(float& lpTargetVar)
{
	target = UI_BAR_TARGET::VARIABLE;
	this->lpTargetVar = &lpTargetVar;
}

void UIProgressBar::SetTrackingData(function<float()> lpTargetFunc)
{
	if (lpTargetFunc)
	{
		target = UI_BAR_TARGET::FUNC;
		this->lpTargetFunc = move(lpTargetFunc);
	}
}

void UIProgressBar::SetTrackingMaxData(float& lpTargetVar)
{
	maxTarget = UI_BAR_TARGET::VARIABLE;
	this->lpMaxVar = &lpTargetVar;
}

void UIProgressBar::SetTrackingMaxData(function<float()> lpTargetFunc)
{
	if (lpTargetFunc)
	{
		maxTarget = UI_BAR_TARGET::FUNC;
		this->lpMaxFunc = move(lpTargetFunc);
	}
}
