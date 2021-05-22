#include "UIProgressBar.h"
#include "LoopHero.h"
#include "Image.h"

void UIProgressBar::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, UI_BAR_TYPE type, string back, string bar)
{
	GameUI::Init(anchor, pos, width, height);

	this->type = type;
	this->target = UI_BAR_TARGET::VARIABLE;
	this->maxVar = 1;

	lpBackground = ImageManager::GetSingleton()->FindImage(back);
	lpBar = ImageManager::GetSingleton()->FindImage(bar);
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
		if (target == UI_BAR_TARGET::VARIABLE)
		{
			if (!lpTargetVar) { lpBar->PatternRender(hdc, rc.left, rc.top, width, height); }
			else
			{
				float ratio = (*lpTargetVar) / maxVar;
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
		}
		else
		{
			if (!lpTargetFunc) { lpBar->PatternRender(hdc, rc.left, rc.top, width, height); }
			else
			{
				float ratio = lpTargetFunc() / maxVar;
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
		}
	}

	GameUI::Render(hdc);
}

void UIProgressBar::SetTrackingData(float* lpTargetVar, float maxVar)
{
	if (lpTargetVar)
	{
		target = UI_BAR_TARGET::VARIABLE;
		this->lpTargetVar = lpTargetVar;
		this->lastVar = *lpTargetVar;
		this->maxVar = (maxVar < 1) ? 1 : maxVar;
	}
}

void UIProgressBar::SetTrackingData(function<float()> lpTargetFunc, float maxVar)
{
	if (lpTargetFunc)
	{
		target = UI_BAR_TARGET::FUNC;
		this->lpTargetFunc = move(lpTargetFunc);
		this->lastVar = this->lpTargetFunc();
		this->maxVar = (maxVar < 1) ? 1 : maxVar;
	}
}