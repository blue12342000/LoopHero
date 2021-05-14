#include "InGame.h"
#include "LoopHero.h"
#include "Image.h"
#include "FieldTileMap.h"
#include "TileTable.h"
#include "InGameRightMenu.h"
#include "InGameHandCard.h"

HRESULT InGame::Init()
{
    lpBuffer = ImageManager::GetSingleton()->FindImage("ingame_backbuffer");
    lpBackImage = ImageManager::GetSingleton()->FindImage("InGame_BackGround");

    lpRightMenu = GameUI::CreateUI<InGameRightMenu>();
    lpRightMenu->Init(UI_ANCHOR::RIGHT_TOP, { 0.0f, 0.0f }, 300, WINSIZE_HEIGHT);

    lpHandCards = GameUI::CreateUI<InGameHandCard>();
    lpHandCards->Init(UI_ANCHOR::LEFT_BOTTOM, { 0.0f, -48.0f  }, WINSIZE_WIDTH - 300, 58 * 2);

    lpFieldTiles = new FieldTileMap();
    lpFieldTiles->Init();

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

    if (lpRightMenu)
    {
        lpRightMenu->Release();
        delete lpRightMenu;
        lpRightMenu = nullptr;
    }

    if (lpHandCards)
    {
        lpHandCards->Release();
        delete lpHandCards;
        lpHandCards = nullptr;
    }
}

void InGame::Update(float deltaTime)
{
    lpHandCards->Update(deltaTime);
    lpRightMenu->Update(deltaTime);
    lpFieldTiles->Update(deltaTime);
}

void InGame::Render(HDC hdc)
{
    HDC hMemDC = lpBuffer->GetMemDC();

    lpBackImage->Render(hMemDC);

    //UI
    lpRightMenu->Render(hMemDC);
    lpFieldTiles->Render(hMemDC);
    lpHandCards->Render(hMemDC);

    lpBuffer->Render(hdc);
}
