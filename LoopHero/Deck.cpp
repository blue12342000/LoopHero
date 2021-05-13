#include "Deck.h"
#include "Image.h"
#include "Card.h"

void Deck::Init()
{
	mCardList.insert(make_pair(CARD_TYPE::ROAD, vector<Card*>()));
	mCardList.insert(make_pair(CARD_TYPE::ROAD_SIDE, vector<Card*>()));
	mCardList.insert(make_pair(CARD_TYPE::ENVIRONMENT, vector<Card*>()));
	mCardList.insert(make_pair(CARD_TYPE::SPECIAL, vector<Card*>()));

	LoadCardData();
}

void Deck::Release()
{
	for (auto pair : mCardList)
	{
		for (Card* lpCard : pair.second)
		{
			delete lpCard;
		}
	}
	mCardList.clear();
}

void Deck::Update(float deltaTime)
{
	for (auto& pair : mCardList)
	{
		for (Card*& lpCard : pair.second)
		{
			lpCard->Update(deltaTime);
		}
	}
}

void Deck::Render(HDC hdc)
{
	for (auto pair : mCardList)
	{
		for (Card* lpCard : pair.second)
		{
			lpCard->Render(hdc);
		}
	}
}

void Deck::LoadCardData()
{
	DataManager::GetSingleton()->LoadIniFile("Ini/card.ini", "card");
	DataManager::GetSingleton()->LoadIniFile("Ini/tiles.ini", "tiles");

	map<string, map<string, string>> mDatas = DataManager::GetSingleton()->GetSingleton()->GetData("card");
	int index = 0;
	for (auto group : mDatas)
	{
		CARD_TYPE type = (CARD_TYPE)stoi(group.second["type"]);
		Card* lpCard = new Card();
		lpCard->Init();
		lpCard->LoadCardInfo(type, group.first, group.second["tileKey"]);
		lpCard->SetPos({ 43.0f + index++ * 74.5f, WINSIZE_HEIGHT - 20.0f });
		mCardList[type].push_back(lpCard);
	}

}
