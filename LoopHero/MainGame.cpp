#include "MainGame.h"
#include "InGame.h"
#include "Timer.h"

HRESULT MainGame::Init()
{
	hdc = GetDC(g_hWnd);
	
	hFont = CreateFont(12, 0, 0, 0, FW_LIGHT, 0, 0, 0, HANGEUL_CHARSET, OUT_DEVICE_PRECIS, 0, DEFAULT_QUALITY, VARIABLE_PITCH | FF_ROMAN, TEXT("±Ã¼­"));

	FontManager::GetSingleton()->Init();
	KeyManager::GetSingleton()->Init();
	ImageManager::GetSingleton()->Init();
	SceneManager::GetSingleton()->Init();
	GameData::GetSingleton()->Init();

	timer = new Timer();
	timer->Init();

    // ¾À¸Þ´ÏÀú µî·Ï
	SceneManager::GetSingleton()->AddScene(SCENE_KIND::INGAME , new InGame());

	SceneManager::GetSingleton()->ChangeScene(SCENE_KIND::INGAME);
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
