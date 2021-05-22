#pragma once
#include "GameUI.h"

class UIHorizontalScroll;
class ObserverHandler;
class InGameHandCard : public GameUI
{
private:
	int moveSpeed;
	POINTFLOAT initPos;
	
	UIHorizontalScroll* lpHScrollView;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Release();
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	void UICardLoot(ObserverHandler* observer);
	void UseCard(ObserverHandler* observer);
	
	virtual void OnMouseEnter(EventData& data) override;
};

