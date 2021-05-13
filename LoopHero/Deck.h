#pragma once
#include "GameObject.h"

class Card;
enum class CARD_TYPE;
class Deck : GameObject
{
private:
	map<CARD_TYPE, vector<Card*>> mCardList;

public:
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

private:
	void LoadCardData();


};