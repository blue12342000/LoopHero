#pragma once
#include "GameUI.h"

enum class UI_BUTTON_STATE
{
	NORMAL,
	PRESS,
	HILIGHT,
	DISABLE
};

enum class UI_BUTTON_TYPE
{
	BUTTON,
	RADIO,
	TOGGLE
};

class Image;
class UIButton : public GameUI
{
private:
	UI_BUTTON_TYPE type;
	UI_BUTTON_STATE state;

	Image* lpButtonImage;
	vector<function<void()>> vClickFuncs;

	string groupId;
	bool isOn;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, UI_BUTTON_TYPE type);
	virtual void Release();
	virtual void Render(HDC hdc);

	void SetButtonImage(string imageKey);
	void SetButtonImage(Image* lpImage);

	void OnClick();
	void CallBack(ObserverHandler* lpCaller);
	void RadioCallBack(ObserverHandler* lpCaller);

	void ClearFunc();
	void PushBackFunc(function<void()> func);
	void PopFunc();

	void SetRadioGroup(string groupId);

	virtual void OnClick(EventData& data) final;
	virtual void OnMouseOver(EventData& data) final;
	virtual void OnMouseOut(EventData& data) final;

	inline void SetState(UI_BUTTON_STATE state) { this->state = state; }
	inline void SetOn(bool isOn) { this->isOn = (isOn && type == UI_BUTTON_TYPE::TOGGLE); }
	inline bool IsOn() { return isOn; }
};

