#pragma once
#include "GameUI.h"

enum class EDIT_MENU
{
	TITLE,
	INGAME,
	EXIT,
	NONE
};

class Image;
class UIButton;
class UIEditMenu : public GameUI
{
private:
	Image* lpBackground;
	UIButton* lpButton[(int)EDIT_MENU::NONE];

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Release() override;
	virtual void Render(HDC hdc) override;

	void SetEvent(EDIT_MENU scene, function<void()> func);
};

