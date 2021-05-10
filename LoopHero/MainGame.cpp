#include "MainGame.h"
#include "InGame.h"
#include "Timer.h"

HRESULT MainGame::Init()
{
	hdc = GetDC(g_hWnd);
	KeyManager::GetSingleton()->Init();
	ImageManager::GetSingleton()->Init();
	SceneManager::GetSingleton()->Init();

	timer = new Timer();
	timer->Init();

    // 씬메니저 등록
	SceneManager::GetSingleton()->AddScene(SCENE_KIND::INGAME , new InGame());

	SceneManager::GetSingleton()->ChangeScene(SCENE_KIND::INGAME);
    return S_OK;
}

void MainGame::Release()
{
	delete timer;
	timer = nullptr;

	ReleaseDC(g_hWnd, hdc);
}

void MainGame::Update()
{
	timer->Tick();
	SceneManager::GetSingleton()->Update(timer->GetDeltaTime());
}

void MainGame::Render()
{
	SceneManager::GetSingleton()->Render(hdc);
	timer->Render(hdc);
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
