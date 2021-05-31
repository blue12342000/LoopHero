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
#include "Hero.h"
#include "EventSystem.h"
#include "BattleField.h"
#include "UIBattleUnit.h"

HRESULT InGame::Init()
{
    Hero* lpHero = GameObject::Create<Hero>();
    lpHero->NewHero("Warrior");

    GameData::GetSingleton()->SetHero(lpHero);

    lpBuffer = ImageManager::GetSingleton()->FindImage("ingame_backbuffer");
    lpBackImage = ImageManager::GetSingleton()->FindImage("InGame_BackGround");

    lpCanvus = GameUI::Create<GameUI>();
    lpCanvus->Init(UI_ANCHOR::LEFT_TOP, { 0.0f, 0.0f }, WINSIZE_WIDTH, WINSIZE_HEIGHT);
    lpCanvus->SetEventCatch(EVENT_CATCH::PASS);

    lpBattleWindow = GameUI::Create<UIBattleWindow>(lpCanvus);
    lpBattleWindow->Init(UI_ANCHOR::MIDDLE, { -100, 0.0f }, 301 * 2, 257 * 2);
    lpBattleWindow->SetVisible(false);

    InGameRightMenu* lpRightMenu = GameUI::Create<InGameRightMenu>(lpCanvus);
    lpRightMenu->Init(UI_ANCHOR::RIGHT_TOP, { 0.0f, 0.0f }, 296, WINSIZE_HEIGHT);
    lpRightMenu->SetEventCatch(EVENT_CATCH::BLOCK_PASS);

    InGameHandCard* lpHandCards = GameUI::Create<InGameHandCard>(lpCanvus);
    lpHandCards->Init(UI_ANCHOR::LEFT_BOTTOM, { 0.0f, -48.0f }, WINSIZE_WIDTH - 300, 58 * 2);

    InGameEventTimer* lpEvent = GameUI::Create<InGameEventTimer>(lpCanvus);
    lpEvent->Init(UI_ANCHOR::LEFT_TOP, POINTFLOAT{ 0.0f, 0.0f }, 121 * 2, 27 * 2);

    lpFieldTiles = GameObject::Create<FieldTileMap>();
    lpFieldTiles->SetHero(GameData::GetSingleton()->GetHero());

    lpEventSystem = PoolingManager::GetSingleton()->GetClass<EventSystem>();
    lpEventSystem->Init();
    lpEventSystem->SetGameUI(lpCanvus);
    lpEventSystem->SetGameObject(lpFieldTiles);
    SetBkMode(lpBuffer->GetMemDC(), TRANSPARENT);
    return S_OK;
}

void InGame::Release()
{
    if (lpFieldTiles)
    {
        lpFieldTiles->Release();
        lpFieldTiles = nullptr;
    }

    if (lpCanvus)
    {
        lpCanvus->Release();
        lpCanvus = nullptr;
    }

    if (lpEventSystem)
    {
        lpEventSystem->Release();
        lpEventSystem = nullptr;
    }

    GameData::GetSingleton()->GetHero()->Release();
    GameData::GetSingleton()->SetHero(nullptr);
    ObserverManager::GetSingleton()->Release();
}

void InGame::Update(float deltaTime)
{
    ObserverManager::GetSingleton()->ProcessingMessage();

    if (KeyManager::GetSingleton()->IsKeyOnceDown('1'))
    {
        SceneManager::GetSingleton()->ChangeScene(SCENE_KIND::TITLE);
        return;
    }

    if (KeyManager::GetSingleton()->IsKeyOnceDown('2'))
    {
        SceneManager::GetSingleton()->ChangeScene(SCENE_KIND::INGAME);
        return;
    }

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

    //ImageManager::GetSingleton()->FindImage("layer_background")->LoopRender(hMemDC, POINT{ WINSIZE_WIDTH / 2, WINSIZE_HEIGHT / 2 }, 300, 400, 0, IMAGE_ALIGN::CENTER);

    lpBuffer->Render(hdc);
}
