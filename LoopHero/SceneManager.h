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

enum class LOAD_STYLE
{
	FADE_OUT,
	NONE
};

class Scene;
class SceneManager : public Singleton<SceneManager>
{
private:
	map<LOAD_STYLE, Scene*> mLpLoadScenes;
	map<SCENE_KIND, Scene*> mLpScenes;
	DWORD loadingThreadId;

public:
	static Scene* lpCurrScene;
	static Scene* lpLoadingScene;
	static Scene* lpReadyScene;

public:
	HRESULT Init();
	void Release();
	void Update(float deltaTime);
	void Render(HDC hdc);

	HRESULT AddScene(SCENE_KIND, Scene*);
	HRESULT AddLoadingScene(Scene*);

	void ChangeScene(SCENE_KIND next, bool isLoading = false);

	static DWORD CALLBACK LoadingThread(LPVOID lpThreadParameter);
};

