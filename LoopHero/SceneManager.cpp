#include "SceneManager.h"
#include "LoadingScene.h"
#include "Scene.h"

HRESULT SceneManager::Init()
{
	lpCurrScene = nullptr;
	lpReadyScene = nullptr;
	lpLoadingScene = nullptr;

	return S_OK;
}

void SceneManager::Release()
{
	if (lpCurrScene)
	{
		lpCurrScene->Release();
	}

	for (auto pair : mLpScenes)
	{
		delete pair.second;
	}
	mLpScenes.clear();
}

void SceneManager::Update(float deltaTime)
{
	if (lpLoadingScene && lpLoadingScene->IsLoading()) { lpLoadingScene->Update(deltaTime); }
	else if (lpCurrScene && !lpLoadingScene) { lpCurrScene->Update(deltaTime); }
}

void SceneManager::Render(HDC hdc)
{
	if (lpLoadingScene && lpLoadingScene->IsLoading()) { lpLoadingScene->Render(hdc);  }
	else if (lpCurrScene && !lpLoadingScene) { lpCurrScene->Render(hdc); }
}

HRESULT SceneManager::AddScene(SCENE_KIND kind, Scene* scene)
{
	if (mLpScenes.find(kind) != mLpScenes.end())
	{
		return E_FAIL;
	}

	mLpScenes.insert(std::make_pair(kind, scene));

	return S_OK;
}

HRESULT SceneManager::AddLoadingScene(LOADING_STYLE style, LoadingScene* lpLoadingScene)
{
	if (mLpLoadScenes.find(style) != mLpLoadScenes.end())
	{
		return E_FAIL;
	}

	mLpLoadScenes.insert(std::make_pair(style, lpLoadingScene));
	return S_OK;
}

void SceneManager::ChangeScene(SCENE_KIND next, LOADING_STYLE loading)
{
	auto it = mLpScenes.find(next);
	if (it == mLpScenes.end()) return;

	if (loading != LOADING_STYLE::NONE)
	{
		auto loadIt = mLpLoadScenes.find(loading);
		if (loadIt != mLpLoadScenes.end())
		{
			lpLoadingScene = loadIt->second;
			lpLoadingScene->Init();
			lpReadyScene = it->second;

			HANDLE hThread = CreateThread(nullptr, 0, LoadingThread, (void*)this, 0, &loadingThreadId);
			CloseHandle(hThread);
			return;
		}
	}

	if (lpCurrScene) lpCurrScene->Release();
	lpCurrScene = it->second;
	lpCurrScene->Init();
}

DWORD __stdcall SceneManager::LoadingThread(LPVOID lpThreadParameter)
{
	SceneManager* lpScene = (SceneManager*)(lpThreadParameter);

	lpScene->lpReadyScene->Init();
	lpScene->lpCurrScene->Release();

	lpScene->lpCurrScene = lpScene->lpReadyScene;
	lpScene->lpReadyScene = nullptr;


	while (!lpScene->lpLoadingScene->IsFinished())
	{
		Sleep(100);
	}
	lpScene->lpLoadingScene->Release();
	lpScene->lpLoadingScene = nullptr;

	return 0;
}
