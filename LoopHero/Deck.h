#pragma once
#include "GameObject.h"

class Card;
enum class CARD_TYPE;
enum class CARD_RARE;
class Deck : public GameObject
{
private:
	vector<Card*> vCardList;
	map<CARD_TYPE, vector<Card*>> mCardListByType;
	map<CARD_RARE, vector<Card*>> mCardListByRare;

	vector<Card*> vActiveCards;
	vector<Card*> vDeactiveCards;

public:
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	Card* GetRandomCard();

private:
	Card* CreateCard(string cardId);
	void LoadCardData();
};