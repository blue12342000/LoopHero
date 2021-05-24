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

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, UI_BUTTON_TYPE type);
	virtual void Render(HDC hdc);

	void SetButtonImage(string imageKey);
	void SetButtonImage(Image* lpImage);

	void OnClick();
	void CallBack(ObserverHandler* lpCaller);

	void PushBackFunc(function<void()> func);
	void PopFunc();

	virtual void OnClick(EventData& data) final;
	virtual void OnMouseOver(EventData& data) final;
	virtual void OnMouseOut(EventData& data) final;
};

