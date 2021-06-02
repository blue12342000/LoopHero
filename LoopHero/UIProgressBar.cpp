#include "UIProgressBar.h"
#include "LoopHero.h"
#include "Image.h"
#include "Utill.h"

void UIProgressBar::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, UI_BAR_TYPE type, string back, string bar)
{
	GameUI::Init(anchor, pos, width, height);

	this->type = type;
	this->target = UI_BAR_TARGET::VARIABLE;
	this->maxTarget = UI_BAR_TARGET::VARIABLE;
	this->lpTargetVar = nullptr;
	this->lpMaxVar = nullptr;
	this->tick = -1;

	lpBackground = ImageManager::GetSingleton()->FindImage(back);
	lpBar = ImageManager::GetSingleton()->FindImage(bar);

	this->var = 0.0f;
	lastVar = var;
}

void UIProgressBar::Release()
{
	lpBackground = nullptr;
	lpBar = nullptr;

	lpTargetVar = nullptr;
	lpMaxVar = nullptr;
	lpTargetFunc = nullptr;
	lpMaxFunc = nullptr;
	isDrag = false;
	vChangedFuncs.clear();
	GameUI::Release();
}

void UIProgressBar::Update(float deltaTime)
{
	if (lastVar < var - FLT_EPSILON || lastVar > var + FLT_EPSILON)
	{
		lastVar = var;
		for (const auto& onChangeFunc : vChangedFuncs)
		{
			onChangeFunc(var);
		}
	}

	GameUI::Update(deltaTime);
}

void UIProgressBar::Render(HDC hdc)
{
	//RenderRectangle(hdc, rc);

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
		case UI_BAR_TYPE::RANGE:
			lpBackground->PatternRender(hdc, rc.left, rc.top, width, height);
			break;
		}
	}

	if (lpBar)
	{
		if (type == UI_BAR_TYPE::RANGE)
		{
			if (maxVar - minVar > 0)
			{
				float ratio = (var - minVar) / (maxVar - minVar);
				if (ratio > 1) ratio = 1;
				lpBar->Render(hdc, rc.left + (int)(width * ratio), (rc.top + rc.bottom) / 2, 0, IMAGE_ALIGN::CENTER);
			}
		}
		else
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

float UIProgressBar::GetVar()
{
	if (type == UI_BAR_TYPE::RANGE)
	{
		return var;
	}
	else
	{
		float targetVar;
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
		return targetVar;
	}
}

void UIProgressBar::SetVar(float var)
{
	if (var < minVar) { var = minVar; }
	else if (var > maxVar) { var = maxVar; }
	else if (tick > 0)
	{
		float ratio = (var - minVar) / (maxVar - minVar);
		var = ((int)((((maxVar - minVar) / tick)) * ratio)) * tick + minVar;
	}

	this->var = var;
	lastVar = var;
}

void UIProgressBar::SetRange(float min, float max)
{
	minVar = min;
	maxVar = max;

	if (var < minVar) var = minVar;
	else if (var > maxVar) var = maxVar;
	lastVar = var;
}

void UIProgressBar::ClearFunc()
{
	vChangedFuncs.clear();
}

void UIProgressBar::PushBackFunc(function<void(float)> func)
{
	vChangedFuncs.push_back(move(func));
}

void UIProgressBar::PopFunc()
{
	vChangedFuncs.pop_back();
}

void UIProgressBar::OnBeginDrag(EventData& data)
{
	float ratio = (var - minVar) / (maxVar - minVar);
	if (ratio > 1) ratio = 1;

	RECT inRect = { rc.left + (int)(width * ratio) - 5, rc.top - 2, rc.left + (int)(width * ratio) + lpBar->GetWidth() + 5, rc.bottom + 2 };
	if (PtInRect(&inRect, data.point))
	{
		isDrag = true;
	}
}

void UIProgressBar::OnDrag(EventData& data)
{
	if (isDrag)
	{
		var = (float)(data.point.x - rc.left);
		if (var < 0) { var = minVar; }
		else if (var > width) { var = maxVar; }
		else
		{
			if (tick < 0)
			{
				float ratio = var / width;
				var = (maxVar - minVar) * ratio + minVar;
			}
			else
			{
				float ratio = var / width;
				var = ((int)((((maxVar - minVar) / tick)) * ratio)) * tick + minVar;
			}
		}
	}
}

void UIProgressBar::OnEndDrag(EventData& data)
{
	isDrag = false;
}
