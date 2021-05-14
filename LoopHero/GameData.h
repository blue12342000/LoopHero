#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <vector>

using namespace std;

class Deck;
class GameData : public Singleton<GameData>
{
private:
	Deck* lpDeck;

public:
	HRESULT Init();

	void Save() {}
	void Load() {}

	inline Card* PickCard() { return lpDeck->GetRandomCard(); }
};

