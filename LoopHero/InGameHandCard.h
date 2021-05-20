#pragma once
#include "GameUI.h"

class Card;
class UIProgressBar;
class UISprite;
class ObserverHandler;
class InGameHandCard : public GameUI
{
private:
	int moveSpeed;
	POINTFLOAT initPos;
	
	GameUI* lpHScrollView;
	UIProgressBar* lpProgressBar;

	vector<UISprite*> vDeactiveSprites;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Release();
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	void UICardLoot(ObserverHandler& observer);
};

