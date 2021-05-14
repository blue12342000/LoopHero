#include "GameData.h"
#include "Deck.h"

HRESULT GameData::Init()
{
    lpDeck = new Deck();
    lpDeck->Init();

    return S_OK;
}
