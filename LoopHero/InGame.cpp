#include "InGame.h"
#include "LoopHero.h"
#include "Image.h"
#include "FieldTileMap.h"
#include "TileTable.h"
#include "UISprite.h"
#include "UIProgressBar.h"
#include "InGameRightMenu.h"
#include "InGameHandCard.h"
#include "Card.h"
#include "Unit.h"
#include "TraitsTable.h"
#include "EventSystem.h"

HRESULT InGame::Init()
{
    lpBuffer = ImageManager::GetSingleton()->FindImage("ingame_backbuffer");
    lpBackImage = ImageManager::GetSingleton()->FindImage("InGame_BackGround");

    lpCanvus = GameUI::CreateUI<GameUI>();
    lpCanvus->Init(UI_ANCHOR::LEFT_TOP, { 0.0f, 0.0f }, WINSIZE_WIDTH, WINSIZE_HEIGHT);

    GameUI* lpRightMenu = GameUI::CreateUI<InGameRightMenu>(lpCanvus);
    lpRightMenu->Init(UI_ANCHOR::RIGHT_TOP, { 0.0f, 0.0f }, 296, WINSIZE_HEIGHT);

    GameUI* lpHandCards = GameUI::CreateUI<InGameHandCard>(lpCanvus);
    lpHandCards->Init(UI_ANCHOR::LEFT_BOTTOM, { 0.0f, -48.0f }, WINSIZE_WIDTH - 300, 58 * 2);

   // UIProgressBar* lpProgressBar = GameUI::CreateUI<UIProgressBar>(lpCanvus);
   // lpProgressBar->Init(UI_ANCHOR::LEFT_BOTTOM, { 100.0f, 400.0f }, 200, 20, UI_BAR_TYPE::HORIZON, "battle_unit_statusbar_action", "battle_unit_statusbar_hp");

    lpFieldTiles = new FieldTileMap();
    lpFieldTiles->Init();

    lpUnit = GameData::GetSingleton()->GetTraitTable()->CreateUnit("Warrior");
    //lpEquipItem = EquipItem::CreateEquip(lpUnit->GetTraits());

    GameData::GetSingleton()->SetUnit(lpUnit);

    lpEventSystem = new EventSystem();
    lpEventSystem->Init();
    lpEventSystem->SetGameUI(lpCanvus);

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

    if (lpCanvus)
    {
        lpCanvus->Release();
        delete lpCanvus;
        lpCanvus = nullptr;
    }

    if (lpUnit)
    {
        lpUnit->Release();
        delete lpUnit;
        lpUnit = nullptr;
    }

    if (lpEventSystem)
    {
        lpEventSystem->Release();
        delete lpEventSystem;
        lpEventSystem = nullptr;
    }
}

void InGame::Update(float deltaTime)
{
    lpEventSystem->Update(deltaTime);
    lpCanvus->Update(deltaTime);
    lpFieldTiles->Update(deltaTime);

    lpCanvus->LateUpdate(deltaTime);
}

void InGame::Render(HDC hdc)
{
    HDC hMemDC = lpBuffer->GetMemDC();

    lpBackImage->Render(hMemDC);

    //UI
    lpFieldTiles->Render(hMemDC);

    lpUnit->Render(hMemDC);
    lpCanvus->Render(hMemDC);

    // µð¹ö±×
    lpEventSystem->Render(hMemDC);

    lpBuffer->Render(hdc);
}
