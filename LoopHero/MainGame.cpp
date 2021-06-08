#include "MainGame.h"
#include "InGame.h"
#include "Title.h"
#include "AnimationEdit.h"
#include "FadeOutScene.h"
#include "Timer.h"

HRESULT MainGame::Init()
{
	hdc = GetDC(g_hWnd);
	
	FontManager::GetSingleton()->Init();
	KeyManager::GetSingleton()->Init();
	ImageManager::GetSingleton()->Init();
	SceneManager::GetSingleton()->Init();
	GameData::GetSingleton()->Init();

	timer = new Timer();
	timer->Init();

    // 씬메니저 등록
	SceneManager::GetSingleton()->AddScene(SCENE_KIND::TITLE, new Title());
	SceneManager::GetSingleton()->AddScene(SCENE_KIND::INGAME , new InGame());
	SceneManager::GetSingleton()->AddScene(SCENE_KIND::ANIM_EDIT, new AnimationEdit());

	SceneManager::GetSingleton()->AddLoadingScene(LOADING_STYLE::FADE_OUT, new FadeOutScene());

	SceneManager::GetSingleton()->ChangeScene(SCENE_KIND::ANIM_EDIT);
    return S_OK;
}

void MainGame::Release()
{
	delete timer;
	timer = nullptr;

	FontManager::GetSingleton()->Release();
	FontManager::GetSingleton()->ReleaseSingleton();

	PoolingManager::GetSingleton()->Release();
	PoolingManager::GetSingleton()->ReleaseSingleton();

	KeyManager::GetSingleton()->ReleaseSingleton();

	ImageManager::GetSingleton()->Release();
	ImageManager::GetSingleton()->ReleaseSingleton();

	SceneManager::GetSingleton()->Release();
	SceneManager::GetSingleton()->ReleaseSingleton();

	GameData::GetSingleton()->Release();
	GameData::GetSingleton()->ReleaseSingleton();

	ObserverManager::GetSingleton()->Release();
	ObserverManager::GetSingleton()->ReleaseSingleton();

	ReleaseDC(g_hWnd, hdc);
}

void MainGame::Update()
{
	timer->Tick();
	SceneManager::GetSingleton()->Update(Timer::deltaTime);
}

void MainGame::Render()
{
	SceneManager::GetSingleton()->Render(hdc);
}

LRESULT MainGame::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_MOUSEMOVE:
		KeyManager::GetSingleton()->SetMousePoint(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

    return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
