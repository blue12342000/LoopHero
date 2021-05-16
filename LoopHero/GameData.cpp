#include "GameData.h"
#include "Deck.h"
#include "Card.h"
#include "TraitsTable.h"

HRESULT GameData::Init()
{
    lpDeck = new Deck();
    lpDeck->Init();

    lpTraitTable = new TraitsTable();
    lpTraitTable->LoadTraitsData();

    return S_OK;
}

void GameData::Release()
{
    if (lpDeck)
    {
        lpDeck->Release();
        delete lpDeck;
        lpDeck = nullptr;
    }

    if (lpTraitTable)
    {
        delete lpTraitTable;
        lpTraitTable = nullptr;
    }
}

Card* GameData::PickCard()
{
    return lpDeck->GetRandomCard();
}
