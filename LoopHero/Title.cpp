#include "Title.h"
#include "SceneManager.h"
#include "ImageManager.h"
#include "PoolingManager.h"
#include "EventSystem.h"
#include "Image.h"
#include "AnimationUIController.h"
#include "AnimationMove.h"
#include "UILogo.h"

HRESULT Title::Init()
{
	lpBuffer = ImageManager::GetSingleton()->AddImage("Title_Buffer", WINSIZE_WIDTH, WINSIZE_HEIGHT);
	lpBackImage = ImageManager::GetSingleton()->FindImage("title_background");

    lpCanvus = GameUI::Create<GameUI>();
    lpCanvus->Init(UI_ANCHOR::LEFT_TOP, { 0.0f, 0.0f }, WINSIZE_WIDTH, WINSIZE_HEIGHT);
    
    UILogo* lpLogo = GameUI::Create<UILogo>(lpCanvus);
    lpLogo->Init(UI_ANCHOR::TOP_MIDDLE, { 0.0f, 0.0f }, 195 * 2, 195 * 2);

    lpEventSystem = PoolingManager::GetSingleton()->GetClass<EventSystem>();
    lpEventSystem->Init();
    lpEventSystem->SetGameUI(lpCanvus);

    lpAnimController = PoolingManager::GetSingleton()->GetClass<AnimationUIController>();
    AnimationMove* lpAnimMove = lpAnimController->AddAnimationHandler<AnimationMove>();

	return E_NOTIMPL;
}

void Title::Release()
{
    lpCanvus->Release();
    lpEventSystem->Release();
    lpAnimController->Release();
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

    lpEventSystem->Update(deltaTime);
    lpCanvus->Update(deltaTime);
}

void Title::Render(HDC hdc)
{
	HDC hMemDC = lpBuffer->GetMemDC();
	lpBackImage->Render(hMemDC);

    lpCanvus->Render(hMemDC);
    lpEventSystem->Render(hMemDC);

	lpBuffer->Render(hdc);
}
