#pragma once
#include "GameUI.h"

class Card;
class UICard : public GameUI
{
private:
	Card* lpCard;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Render(HDC hdc) override;

	inline void SetCard(Card* lpCard) { this->lpCard = lpCard; }
};

