#pragma once
#include "GameUI.h"
#include "AnimationUIController.h"

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
	UIButton* lpResetBtn;
	UIButton* lpLinearBtn;

	// 애니메이션을 제어당할 아이
	GameUI* lpTarget;
	AnimationUIController* lpTargetAnim;
	vector<AnimVariable> vAnimVariables;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height);
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);

	void MoveTarget(float time);
	void AnimTimeTickRefreash();

	void OpenAnimController(ObserverHandler* lpCaller);

	inline void SetState(ANIM_INSPECTOR_STATE state) { this->state = state; }
};

