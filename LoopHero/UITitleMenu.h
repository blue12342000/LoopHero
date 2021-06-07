#pragma once
#include "GameUI.h"

class Image;
class UIButton;
class UITitleMenu : public GameUI
{
private:
	Image* lpBackground;
	UIButton* lpStartBtn;
	UIButton* lpEditBtn;
	UIButton* lpExitBtn;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Release() override;
	virtual void Render(HDC hdc) override;

};

