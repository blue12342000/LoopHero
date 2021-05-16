#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <vector>

using namespace std;

class Card;
class Deck;
class TraitsTable;
class GameData : public Singleton<GameData>
{
private:
	Deck* lpDeck;
	TraitsTable* lpTraitTable;
	int loopLevel;

public:
	HRESULT Init();
	void Release();

	void Save() {}
	void Load() {}

	Card* PickCard();
	inline int GetLoopLevel() { return loopLevel; }
};

