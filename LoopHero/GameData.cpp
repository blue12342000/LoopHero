#include "GameData.h"
#include "Deck.h"
#include "Card.h"
#include "TraitsTable.h"
#include "EquipTable.h"

HRESULT GameData::Init()
{
    lpDeck = GameObject::Create<Deck>();
    lpDeck->Init();

    lpTraitTable = new TraitsTable();
    lpTraitTable->LoadTraitsData();

    lpEquipTable = new EquipTable();
    lpEquipTable->LoadEquipData();

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

    if (lpEquipTable)
    {
        delete lpEquipTable;
        lpEquipTable = nullptr;
    }
}

Card* GameData::PickCard()
{
    return lpDeck->GetRandomCard();
}
