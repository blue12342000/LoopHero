#pragma once
#include "GameUI.h"
class UIBattleWindow : public GameUI
{
private:
	Image* lpBackground;


public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
};

