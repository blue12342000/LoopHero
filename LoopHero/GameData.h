#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <vector>

using namespace std;

class Card;
class Deck;
class TraitsTable;
class EquipTable;
class GameData : public Singleton<GameData>
{
private:
	Deck* lpDeck;
	TraitsTable* lpTraitTable;
	EquipTable* lpEquipTable;
	int loopLevel;

public:
	HRESULT Init();
	void Release();

	void Save() {}
	void Load() {}

	Card* PickCard();
	inline int GetLoopLevel() { return loopLevel; }
};

