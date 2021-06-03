#include "Title.h"
#include "SceneManager.h"
#include "ImageManager.h"
#include "PoolingManager.h"
#include "EventSystem.h"
#include "Image.h"
#include "UILogo.h"
#include "UIEditBox.h"
#include "UIAnimInspector.h"
#include "UIDebug.h"

HRESULT Title::Init()
{
	lpBuffer = ImageManager::GetSingleton()->AddImage("Title_Buffer", WINSIZE_WIDTH, WINSIZE_HEIGHT);
	lpBackImage = ImageManager::GetSingleton()->FindImage("title_background");

    lpCanvus = GameUI::Create<GameUI>();
    lpCanvus->Init(UI_ANCHOR::LEFT_TOP, { 0.0f, 0.0f }, WINSIZE_WIDTH, WINSIZE_HEIGHT);
    
    UILogo* lpLogo = GameUI::Create<UILogo>(lpCanvus);
    lpLogo->Init(UI_ANCHOR::TOP_MIDDLE, { 0.0f, 0.0f }, 195 * 2, 195 * 2);

    UIAnimInspector* lpUIAnimInspector = GameUI::Create<UIAnimInspector>(lpCanvus);
    lpUIAnimInspector->Init(UI_ANCHOR::RIGHT_BOTTOM, { 0.0f, 0.0f }, 600, 300);
    lpUIAnimInspector->SetVisible(false);

    UIDebug* lpUIDebug = GameUI::Create<UIDebug>(lpCanvus);
    lpUIDebug->Init(UI_ANCHOR::LEFT_TOP, { 0.0f, 0.0f }, 250, 50);
    lpUIDebug->SetVisible(false);

    lpEventSystem = PoolingManager::GetSingleton()->GetClass<EventSystem>();
    lpEventSystem->Init();
    lpEventSystem->SetGameUI(lpCanvus);

	return E_NOTIMPL;
}

void Title::Release()
{
    lpCanvus->Release();
    lpEventSystem->Release();
}

void Title::Update(float deltaTime)
{
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

    if (KeyManager::GetSingleton()->IsKeyOnceDown('Q'))
    {
        ObserverManager::GetSingleton()->Notify("OpenDebugInfo", lpEventSystem);
    }

    ObserverManager::GetSingleton()->ProcessingMessage();
    lpEventSystem->Update(deltaTime);
    lpCanvus->Update(deltaTime);

    lpCanvus->LateUpdate(deltaTime);
}

void Title::Render(HDC hdc)
{
	HDC hMemDC = lpBuffer->GetMemDC();
	lpBackImage->Render(hMemDC);

    lpCanvus->Render(hMemDC);
    //lpEventSystem->Render(hMemDC);

	lpBuffer->Render(hdc);
}
