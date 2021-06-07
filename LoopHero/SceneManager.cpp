#include "SceneManager.h"
#include "Scene.h"

Scene* SceneManager::lpCurrScene = nullptr;
Scene* SceneManager::lpLoadingScene = nullptr;
Scene* SceneManager::lpReadyScene = nullptr;

HRESULT SceneManager::Init()
{
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
	if (lpCurrScene) lpCurrScene->Update(deltaTime);
}

void SceneManager::Render(HDC hdc)
{
	if (lpCurrScene) lpCurrScene->Render(hdc);
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

HRESULT SceneManager::AddLoadingScene(Scene* a)
{
	mLpLoadScenes[LOAD_STYLE::FADE_OUT] = a;
	return S_OK;
}

void SceneManager::ChangeScene(SCENE_KIND next, bool isLoading)
{
	auto it = mLpScenes.find(next);
	if (it == mLpScenes.end()) return;
	if (isLoading)
	{
		lpLoadingScene = it->second;

		mLpLoadScenes[LOAD_STYLE::FADE_OUT]->Init();
		if (lpCurrScene) lpCurrScene->Release();
		lpCurrScene = mLpLoadScenes[LOAD_STYLE::FADE_OUT];

		HANDLE hThread = CreateThread(nullptr, 0, LoadingThread/* ÇÔ¼ö */, (void*)this, 0, &loadingThreadId);
		CloseHandle(hThread);
		return;
	}

	if (lpCurrScene) lpCurrScene->Release();
	lpCurrScene = it->second;
	lpCurrScene->Init();
}

DWORD __stdcall SceneManager::LoadingThread(LPVOID lpThreadParameter)
{
	SceneManager* lpScene = (SceneManager*)(lpThreadParameter);


	return 0;
}
