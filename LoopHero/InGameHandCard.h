#pragma once
#include "GameUI.h"

class Card;
class InGameHandCard : public GameUI
{
private:
	int moveSpeed;
	POINTFLOAT initPos;
	
	vector<Card*> vHandCards;
	vector<Card*> vDropCards;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
};

