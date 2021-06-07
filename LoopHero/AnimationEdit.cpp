#include "AnimationEdit.h"
#include "SceneManager.h"
#include "ImageManager.h"
#include "PoolingManager.h"
#include "EventSystem.h"
#include "Image.h"
#include "UILogo.h"
#include "UITitleMenu.h"
#include "InGameRightMenu.h"
#include "InGameHandCard.h"
#include "InGameEventTimer.h"
#include "UIBattleWindow.h"
#include "UIAnimInspector.h"
#include "UIDebug.h"
#include "UIHierarchy.h"
#include "UIEditMenu.h"

HRESULT AnimationEdit::Init()
{
    lpBuffer = ImageManager::GetSingleton()->AddImage("Title_Buffer", WINSIZE_WIDTH, WINSIZE_HEIGHT);
    lpBackImage = ImageManager::GetSingleton()->FindImage("title_background");

    DataManager::GetSingleton()->SaveIniFile("Ini/animation.ini", "animations");

    lpCanvus = GameUI::Create<GameUI>();
    lpCanvus->Init(UI_ANCHOR::LEFT_TOP, { 0.0f, 0.0f }, WINSIZE_WIDTH, WINSIZE_HEIGHT);

    UIEditMenu* lpUIEditMenu = GameUI::Create<UIEditMenu>(lpCanvus);
    lpUIEditMenu->Init(UI_ANCHOR::TOP_MIDDLE, { 0.0f, 0.0f }, 200 * 2, 27 * 2);
    lpUIEditMenu->SetEvent(EDIT_MENU::TITLE, bind(&AnimationEdit::LoadTitleUI, this));
    lpUIEditMenu->SetEvent(EDIT_MENU::INGAME, bind(&AnimationEdit::LoadInGameUI, this));
    lpUIEditMenu->SetEvent(EDIT_MENU::EXIT, bind(&SceneManager::ChangeScene, SceneManager::GetSingleton(), SCENE_KIND::TITLE, true));

    lpUIAnimInspector = GameUI::Create<UIAnimInspector>(lpCanvus);
    lpUIAnimInspector->Init(UI_ANCHOR::RIGHT_BOTTOM, { 0.0f, 0.0f }, 600, 300);
    lpUIAnimInspector->SetVisible(false);

    lpHierarchy = GameUI::Create<UIHierarchy>(lpCanvus);
    lpHierarchy->Init(UI_ANCHOR::LEFT_BOTTOM, { 0, 0 }, 300, 500);
    lpHierarchy->SetVisible(false);

    lpEventSystem = PoolingManager::GetSingleton()->GetClass<EventSystem>();
    lpEventSystem->Init();
    lpEventSystem->SetGameUI(lpCanvus);

    lpUIDebug = GameUI::Create<UIDebug>(lpCanvus);
    lpUIDebug->Init(UI_ANCHOR::LEFT_TOP, { 0.0f, 0.0f }, 250, 50);
    lpUIDebug->OpenDebugInfo(lpEventSystem);
    lpUIDebug->SetVisible(false);

    LoadTitleUI();

	return S_OK;
}

void AnimationEdit::Release()
{
    lpCanvus->Release();
    lpEventSystem->Release();
}

void AnimationEdit::Update(float deltaTime)
{
    if (KeyManager::GetSingleton()->IsKeyOnceDown('Q'))
    {
        lpUIDebug->SetVisible(!lpUIDebug->IsVisible());
    }

    if (KeyManager::GetSingleton()->IsKeyOnceDown(VK_SPACE))
    {
        // ÇÏÀÌ¶óÅ°ºä ¿ÀÇÂ
        lpHierarchy->SetVisible(!lpHierarchy->IsVisible());
    }

    ObserverManager::GetSingleton()->ProcessingMessage();
    lpEventSystem->Update(deltaTime);
    lpCanvus->Update(deltaTime);
    lpCanvus->LateUpdate(deltaTime);
}

void AnimationEdit::Render(HDC hdc)
{
    HDC hMemDC = lpBuffer->GetMemDC();
    lpBackImage->Render(hMemDC);
    lpCanvus->Render(hMemDC);
    lpBuffer->Render(hdc);
}

void AnimationEdit::ResetInit()
{
    vector<GameUI*> vCanvusUI = move(lpCanvus->GetChilds());
    for (const auto& lpGameUI : vCanvusUI)
    {
        if (typeid(*lpGameUI) == typeid(UIAnimInspector)
            || typeid(*lpGameUI) == typeid(UIHierarchy)
            || typeid(*lpGameUI) == typeid(UIDebug)
            || typeid(*lpGameUI) == typeid(UIEditMenu)) continue;

        lpCanvus->RemoveChild(lpGameUI);
    }

    lpUIAnimInspector->SetVisible(false);
    lpHierarchy->SetVisible(false);
}

void AnimationEdit::LoadTitleUI()
{
    ResetInit();

    lpBuffer = ImageManager::GetSingleton()->AddImage("Title_Buffer", WINSIZE_WIDTH, WINSIZE_HEIGHT);
    lpBackImage = ImageManager::GetSingleton()->FindImage("title_background");

    UILogo* lpLogo = GameUI::Create<UILogo>();
    lpLogo->Init(UI_ANCHOR::TOP_MIDDLE, { 0.0f, 0.0f }, 195 * 2, 195 * 2);
    lpCanvus->InsertChild(lpLogo, 0);

    UITitleMenu* lpTitleMenu = GameUI::Create<UITitleMenu>();
    lpTitleMenu->Init(UI_ANCHOR::TOP_MIDDLE, { 0.0f, 0.0f }, 115 * 2, 376 * 2);
    lpCanvus->InsertChild(lpTitleMenu, 0);

    lpHierarchy->SetCanvus(lpCanvus);
}

void AnimationEdit::LoadInGameUI()
{
    ResetInit();

    lpBuffer = ImageManager::GetSingleton()->FindImage("ingame_backbuffer");
    lpBackImage = ImageManager::GetSingleton()->FindImage("InGame_BackGround");

    UIBattleWindow* lpBattleWindow = GameUI::Create<UIBattleWindow>();
    lpBattleWindow->Init(UI_ANCHOR::MIDDLE, { -100, 0.0f }, 301 * 2, 257 * 2);
    lpBattleWindow->SetVisible(false);
    lpCanvus->InsertChild(lpBattleWindow, 0);

    InGameRightMenu* lpRightMenu = GameUI::Create<InGameRightMenu>();
    lpRightMenu->Init(UI_ANCHOR::RIGHT_TOP, { 0.0f, 0.0f }, 296, WINSIZE_HEIGHT);
    lpCanvus->InsertChild(lpRightMenu, 0);

    InGameHandCard* lpHandCards = GameUI::Create<InGameHandCard>();
    lpHandCards->Init(UI_ANCHOR::LEFT_BOTTOM, { 0.0f, -48.0f }, WINSIZE_WIDTH - 300, 58 * 2);
    lpCanvus->InsertChild(lpHandCards, 0);
    
    InGameEventTimer* lpEvent = GameUI::Create<InGameEventTimer>();
    lpEvent->Init(UI_ANCHOR::LEFT_TOP, POINTFLOAT{ 0.0f, 0.0f }, 121 * 2, 27 * 2);
    lpCanvus->InsertChild(lpEvent, 0);

    lpHierarchy->SetCanvus(lpCanvus);
}
