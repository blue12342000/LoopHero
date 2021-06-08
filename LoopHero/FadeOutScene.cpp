#include "FadeOutScene.h"
#include "LoopHero.h"
#include "Image.h"
#include "Utill.h"

HRESULT FadeOutScene::Init()
{
    loadingState = LOADING_STATE::LOADING;
    lpBuffer = ImageManager::GetSingleton()->AddImage("FadeOut_Buffer", WINSIZE_WIDTH, WINSIZE_HEIGHT);
    lpInBuffer = ImageManager::GetSingleton()->AddImage("FadeIn_Buffer", WINSIZE_WIDTH, WINSIZE_HEIGHT);

    // 이전 화면정보 Get
    SceneManager::GetSingleton()->GetCurrentScene()->Render(lpBuffer->GetMemDC());

    splitView.resize(VIEW_SPLIT_X * VIEW_SPLIT_Y);
    for (int i = 0, n = VIEW_SPLIT_X * VIEW_SPLIT_Y; i < n; ++i)
    {
        splitView[i] = i;
    }
    random_shuffle(splitView.begin(), splitView.end());

    width = (int)ceil(((float)WINSIZE_WIDTH) / VIEW_SPLIT_X);
    height = (int)ceil(((float)WINSIZE_HEIGHT) / VIEW_SPLIT_Y);
    lastProcess = -1;
    elapsedTime = 0.0f;
    fadeSpeed = 2.0f;

    inOut = FADE_IN_OUT::FADE_OUT;
    return S_OK;
}

void FadeOutScene::Release()
{
    loadingState = LOADING_STATE::DEACTIVE;
    splitView.clear();
    elapsedTime = 0.0f;
}

void FadeOutScene::Update(float deltaTime)
{
    if (loadingState != LOADING_STATE::LOADING) return;

    if (inOut == FADE_IN_OUT::FADE_IN && elapsedTime > fadeSpeed)
    {
        loadingState = LOADING_STATE::FINISHED;
        return;
    }

    elapsedTime += deltaTime;
    if (inOut == FADE_IN_OUT::FADE_OUT && !SceneManager::GetSingleton()->IsLoadNextScene())
    {
        SceneManager::GetSingleton()->GetCurrentScene()->Render(lpInBuffer->GetMemDC());
        inOut = FADE_IN_OUT::FADE_IN;
        elapsedTime = 0.0f;
        lastProcess = -1;

        random_shuffle(splitView.begin(), splitView.end());
    }
}

void FadeOutScene::Render(HDC hdc)
{
    if (loadingState != LOADING_STATE::LOADING) return;

    if (inOut == FADE_IN_OUT::FADE_OUT)
    {
        for (int i = lastProcess + 1, n = (int)(LerpAxis(0, VIEW_SPLIT_X * VIEW_SPLIT_Y, elapsedTime / fadeSpeed) + FLT_EPSILON); i < n; ++i)
        {
            lpBuffer->Fill(splitView[i] % VIEW_SPLIT_X * width, splitView[i] / VIEW_SPLIT_X * height, width, height);
            lastProcess = i;
        }
    }
    else
    {
        for (int i = lastProcess + 1, n = (int)(LerpAxis(0, VIEW_SPLIT_X * VIEW_SPLIT_Y, elapsedTime / fadeSpeed) + FLT_EPSILON); i < n; ++i)
        {
            lpBuffer->Fill(lpInBuffer->GetMemDC(), splitView[i] % VIEW_SPLIT_X * width, splitView[i] / VIEW_SPLIT_X * height, width, height);
            lastProcess = i;
        }
    }
    lpBuffer->Render(hdc);
}
