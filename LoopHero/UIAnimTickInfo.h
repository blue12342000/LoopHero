#pragma once
#include "GameUI.h"
#include "AnimationUIController.h"

class UITextField;
class UIAnimTickInfo : public GameUI
{
private:
	Image* lpBackground;
	Image* lpTickArrow;

	UITextField* lpTickLabel;
	UITextField* lpTickPoint;

	UITextField* lpTimeLabel;
	UITextField* lpElapsedTime;

	vector<AnimVariable> vAnimVariables;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Render(HDC hdc) override;

	void ViewAnimVariable(float time);

	inline void SetAnimVariables(vector<AnimVariable>& vAnimVariables) { this->vAnimVariables = vAnimVariables; }
};

