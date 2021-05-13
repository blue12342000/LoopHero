#include "InGame.h"
#include "LoopHero.h"
#include "Image.h"
#include "FieldTileMap.h"
#include "TileTable.h"
#include "Deck.h"
#include "InGameRightMenu.h"
#include "Utill.h"

HRESULT InGame::Init()
{
    lpBuffer = ImageManager::GetSingleton()->FindImage("ingame_backbuffer");
    lpBackImage = ImageManager::GetSingleton()->FindImage("InGame_BackGround");

    lpRightMenu = GameUI::CreateUI<InGameRightMenu>();
    lpRightMenu->Init(UI_ANCHOR::RIGHT_TOP, UI_EVENT_CAPTURE::PASS, {0, 0}, 300, WINSIZE_HEIGHT);

    lpFieldTiles = new FieldTileMap();
    lpFieldTiles->Init();

    lpDeck = new Deck();
    lpDeck->Init();

    SetBkMode(lpBuffer->GetMemDC(), TRANSPARENT);
    return S_OK;
}

void InGame::Release()
{
    if (lpFieldTiles)
    {
        lpFieldTiles->Release();
        delete lpFieldTiles;
        lpFieldTiles = nullptr;
    }

    if (lpDeck)
    {
        lpDeck->Release();
        delete lpDeck;
        lpDeck = nullptr;
    }

    if (lpRightMenu)
    {
        lpRightMenu->Release();
        delete lpRightMenu;
        lpRightMenu = nullptr;
    }
}

void InGame::Update(float deltaTime)
{
    lpRightMenu->Update(deltaTime);
    lpFieldTiles->Update(deltaTime);
    lpDeck->Update(deltaTime);
}

void InGame::Render(HDC hdc)
{
    HDC hMemDC = lpBuffer->GetMemDC();

    lpBackImage->Render(hMemDC);
    lpRightMenu->Render(hMemDC);

    lpFieldTiles->Render(hMemDC);
    lpDeck->Render(hMemDC);

    lpBuffer->Render(hdc);
}
