#include "InGame.h"
#include "LoopHero.h"
#include "Image.h"
#include "FieldTileMap.h"
#include "UISprite.h"
#include "UIProgressBar.h"
#include "InGameRightMenu.h"
#include "InGameHandCard.h"
#include "InGameEventTimer.h"
#include "UIBattleWindow.h"
#include "Card.h"
#include "Unit.h"
#include "Trait.h"
#include "EventSystem.h"
#include "BattleField.h"
#include "UIBattleUnit.h"

HRESULT InGame::Init()
{
    GameData::GetSingleton()->SetUnit(Trait::NewUnit("Warrior"));

    lpBuffer = ImageManager::GetSingleton()->FindImage("ingame_backbuffer");
    lpBackImage = ImageManager::GetSingleton()->FindImage("InGame_BackGround");

    lpCanvus = GameUI::CreateUI<GameUI>();
    lpCanvus->Init(UI_ANCHOR::LEFT_TOP, { 0.0f, 0.0f }, WINSIZE_WIDTH, WINSIZE_HEIGHT);
    lpCanvus->SetEventCatch(EVENT_CATCH::PASS);

    InGameRightMenu* lpRightMenu = GameUI::CreateUI<InGameRightMenu>(lpCanvus);
    lpRightMenu->Init(UI_ANCHOR::RIGHT_TOP, { 0.0f, 0.0f }, 296, WINSIZE_HEIGHT);
    lpRightMenu->SetEventCatch(EVENT_CATCH::BLOCK_PASS);

    InGameHandCard* lpHandCards = GameUI::CreateUI<InGameHandCard>(lpCanvus);
    lpHandCards->Init(UI_ANCHOR::LEFT_BOTTOM, { 0.0f, -48.0f }, WINSIZE_WIDTH - 300, 58 * 2);

    InGameEventTimer* lpEvent = GameUI::CreateUI<InGameEventTimer>(lpCanvus);
    lpEvent->Init(UI_ANCHOR::LEFT_TOP, POINTFLOAT{ 0.0f, 0.0f }, 121 * 2, 27 * 2);

    lpFieldTiles = GameObject::Create<FieldTileMap>();
    lpFieldTiles->Init();

    lpEventSystem = new EventSystem();
    lpEventSystem->Init();
    lpEventSystem->SetGameUI(lpCanvus);
    lpEventSystem->SetGameObject(lpFieldTiles);

    lpBattleWindow = GameUI::CreateUI<UIBattleWindow>(lpCanvus);
    lpBattleWindow->Init(UI_ANCHOR::MIDDLE, { -100, 0.0f }, 301 * 2, 257 * 2);
    lpBattleWindow->SetVisible(false);

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

    if (lpEventSystem)
    {
        lpEventSystem->Release();
        delete lpEventSystem;
        lpEventSystem = nullptr;
    }

    ObserverManager::GetSingleton()->Release();
}

void InGame::Update(float deltaTime)
{
    ObserverManager::GetSingleton()->ProcessingMessage();

    if (KeyManager::GetSingleton()->IsKeyOnceDown('B'))
    {
        lpBattleWindow->SetVisible(!lpBattleWindow->IsVisible());
    }

    lpEventSystem->Update(deltaTime);
    lpCanvus->Update(deltaTime);
    lpFieldTiles->Update(deltaTime);
    //GameData::GetSingleton()->GetUnit()->Update(deltaTime);

    lpCanvus->LateUpdate(deltaTime);
}

void InGame::Render(HDC hdc)
{
    HDC hMemDC = lpBuffer->GetMemDC();

    lpBackImage->Render(hMemDC);

    //UI
    lpFieldTiles->Render(hMemDC);

    lpCanvus->Render(hMemDC);

    // µð¹ö±×
    lpEventSystem->Render(hMemDC);

    lpBuffer->Render(hdc);
}
