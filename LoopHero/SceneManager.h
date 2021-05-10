#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <map>

enum class SCENE_KIND
{
	TITLE,
	LOBBY,
	INGAME,
	NONE
};

class Scene;
class SceneManager : public Singleton<SceneManager>
{
private:
	Scene* lpLoadingScene;
	std::map<SCENE_KIND, Scene*> mLpScenes;

public:
	static Scene* lpCurrScene;
	static Scene* lpReadyScene;

public:
	HRESULT Init();
	void Release();
	void Update(float deltaTime);
	void Render(HDC hdc);

	HRESULT AddScene(SCENE_KIND, Scene*);
	HRESULT AddLoadingScene(Scene*);

	void ChangeScene(SCENE_KIND next, bool isLoading = false);
};

