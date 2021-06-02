#pragma once
#include "GameUI.h"

enum class ANIM_INSPECTOR_STATE
{
	STOP,
	PLAY
};

class Image;
class UIGameUIInfo;
class UIButton;
class UIProgressBar;
class UIHorizontalScroll;
class UITextField;
class UIAnimTickInfo;
class UIAnimInspector : public GameUI
{
private:
	ANIM_INSPECTOR_STATE state;
	Image* lpBackground;

	UIGameUIInfo* lpUIInfo;
	UIProgressBar* lpTimeBar;
	UITextField* lpTimeLabel;
	UIHorizontalScroll* lpAnimTimeScroll;
	UIAnimTickInfo* lpAnimTickInfo;

	UIButton* lpSaveBtn;
	UIButton* lpPlayBtn;
	UIButton* lpPauseBtn;

	GameUI* lpTarget;
	AnimationUIController* lpTargetAnim;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height);
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);

	void OpenAnimController(ObserverHandler* lpCaller);

	inline void SetState(ANIM_INSPECTOR_STATE state) { this->state = state; }
};

