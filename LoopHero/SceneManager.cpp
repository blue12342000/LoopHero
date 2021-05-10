#include "SceneManager.h"
#include "Scene.h"

Scene* SceneManager::lpCurrScene = nullptr;
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

HRESULT SceneManager::AddLoadingScene(Scene*)
{

	return S_OK;
}

void SceneManager::ChangeScene(SCENE_KIND next, bool isLoading)
{
	auto it = mLpScenes.find(next);
	if (it == mLpScenes.end()) return;
	if (isLoading)
	{
		// 스레드 사용
		return;
	}

	if (lpCurrScene) lpCurrScene->Release();
	lpCurrScene = it->second;
	lpCurrScene->Init();
}
