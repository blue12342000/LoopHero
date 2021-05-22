#pragma once
#include "GameUI.h"

enum class UI_BAR_TYPE
{
	HORIZON,
	VERTICAL
};

enum class UI_BAR_TARGET
{
	VARIABLE,
	FUNC
};

class Image;
class UIProgressBar : public GameUI
{
private:
	UI_BAR_TYPE type;
	UI_BAR_TARGET target;

	Image* lpBackground;
	Image* lpBar;

	function<float()> lpTargetFunc;
	float* lpTargetVar;
	float maxVar;
	float lastVar;

public:
	virtual ~UIProgressBar() {}

	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, UI_BAR_TYPE type, string back, string bar);

	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	void SetTrackingData(float* lpTargetVar, float maxVar);
	void SetTrackingData(function<float()> lpTargetFunc, float maxVar);
};

