#include "FadeOutScene.h"
#include "LoopHero.h"
#include "Image.h"
#include "Utill.h"

HRESULT FadeOutScene::Init()
{
    lpBuffer = ImageManager::GetSingleton()->AddImage("FadeOut_Buffer", WINSIZE_WIDTH, WINSIZE_HEIGHT);

    // 이전 화면정보 Get
    SceneManager::lpCurrScene->Render(lpBuffer->GetMemDC());

    splitView.resize(VIEW_SPLIT_X * VIEW_SPLIT_Y);
    for (int i = 0, n = VIEW_SPLIT_X * VIEW_SPLIT_Y; i < n; ++i)
    {
        splitView[i] = i;
    }
    random_shuffle(splitView.begin(), splitView.end());

    width = (int)ceil(((float)WINSIZE_WIDTH) / VIEW_SPLIT_X);
    height = (int)ceil(((float)WINSIZE_HEIGHT) / VIEW_SPLIT_Y);
    lastProcess = -1;

    inOut = FADE_IN_OUT::FADE_OUT;

    return S_OK;
}

void FadeOutScene::Release()
{
    splitView.clear();
}

void FadeOutScene::Update(float deltaTime)
{
    elapsedTime += deltaTime;
}

void FadeOutScene::Render(HDC hdc)
{
    for (int i = lastProcess + 1, n = (int)(LerpAxis(0, VIEW_SPLIT_X * VIEW_SPLIT_Y, elapsedTime / 2.0f) + FLT_EPSILON); i < n; ++i)
    {
        lpBuffer->Fill(splitView[i] % VIEW_SPLIT_X * width, splitView[i] / VIEW_SPLIT_X * height, width, height);
        lastProcess = i;
    }
    lpBuffer->Render(hdc);
}
