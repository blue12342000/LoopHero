#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <vector>

using namespace std;

class Card;
class Deck;
class TraitsTable;
class EquipTable;
class Unit;
class GameData : public Singleton<GameData>
{
private:
	Deck* lpDeck;
	TraitsTable* lpTraitTable;
	EquipTable* lpEquipTable;
	int loopLevel;

	Unit* lpUnit;

public:
	HRESULT Init();
	void Release();

	void Save() {}
	void Load() {}

	Card* PickCard();
	inline int GetLoopLevel() { return loopLevel; }
	inline EquipTable* GetEquipTable() { return lpEquipTable; }
	inline TraitsTable* GetTraitTable() { return lpTraitTable; }
	inline void SetUnit(Unit* lpUnit) { this->lpUnit = lpUnit; }
	inline Unit* GetUnit() { return lpUnit; }
};

