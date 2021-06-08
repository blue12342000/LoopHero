#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <map>
#include <functional>
#include <thread>

using namespace std;

enum class SCENE_KIND
{
	TITLE,
	INGAME,
	ANIM_EDIT,
	NONE
};

enum class LOADING_STYLE
{
	FADE_OUT,
	NONE
};

class Scene;
class LoadingScene;
class SceneManager : public Singleton<SceneManager>
{
private:
	map<LOADING_STYLE, LoadingScene*> mLpLoadScenes;
	map<SCENE_KIND, Scene*> mLpScenes;
	DWORD loadingThreadId;

	Scene* lpCurrScene;
	Scene* lpReadyScene;
	LoadingScene* lpLoadingScene;

public:
	HRESULT Init();
	void Release();
	void Update(float deltaTime);
	void Render(HDC hdc);

	HRESULT AddScene(SCENE_KIND kind, Scene* lpScene);
	HRESULT AddLoadingScene(LOADING_STYLE style, LoadingScene* lpLoadingScene);

	void ChangeScene(SCENE_KIND next, LOADING_STYLE loading = LOADING_STYLE::NONE);

	inline bool IsLoadNextScene() { return lpReadyScene != nullptr; }
	inline Scene* GetCurrentScene() { return lpCurrScene; }

	static DWORD CALLBACK LoadingThread(LPVOID lpThreadParameter);
};

