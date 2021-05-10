#include "InGame.h"
#include "LoopHero.h"
#include "Image.h"
#include "FieldTileMap.h"

HRESULT InGame::Init()
{
    lpBuffer = ImageManager::GetSingleton()->FindImage("ingame_backbuffer");
    lpBackImage = ImageManager::GetSingleton()->FindImage("InGame_BackGround");

    lpFieldTiles = new FieldTileMap();
    lpFieldTiles->Init();

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
}

void InGame::Update(float deltaTime)
{
    lpFieldTiles->Update(deltaTime);
}

void InGame::Render(HDC hdc)
{
    HDC hMemDC = lpBuffer->GetMemDC();

    lpBackImage->Render(hMemDC);
    lpFieldTiles->Render(hMemDC);

    lpBuffer->Render(hdc);
}
