#include "LoopHero.h"
#include "MainGame.h"

HINSTANCE g_hInstance;
HWND g_hWnd;
LPSTR g_lpszClass = (LPSTR)TEXT("Loop Hero");
MainGame g_mainGame;
bool isDebugMode = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void SetWindowSize(HWND hWnd, int width, int height);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd)
{
	g_hInstance = hInstance;
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITENESS);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = g_hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = g_lpszClass;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndClass);

	g_hWnd = CreateWindow(g_lpszClass, g_lpszClass, WS_OVERLAPPEDWINDOW, WINPOS_STARTX, WINPOS_STARTY, WINSIZE_WIDTH, WINSIZE_HEIGHT, NULL, NULL, g_hInstance, NULL);
	SetWindowSize(g_hWnd, WINSIZE_WIDTH, WINSIZE_HEIGHT);

	ShowWindow(g_hWnd, nShowCmd);

	if (FAILED(g_mainGame.Init()))
	{
		MessageBox(g_hWnd, "게임초기화에 실패했습니다", "에러", MB_OK);
		return 0;
	}

	MSG message;
	while (true)
	{
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			g_mainGame.Update();
			g_mainGame.Render();
		}
	}
	g_mainGame.Release();
	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return g_mainGame.WndProc(hWnd, iMessage, wParam, lParam);
}

void SetWindowSize(HWND hWnd, int width, int height)
{
	RECT wndRect;
	DWORD wndStyle = GetWindowLong(hWnd, GWL_STYLE);

	SetRect(&wndRect, 0, 0, width, height);
	AdjustWindowRect(&wndRect, wndStyle, GetMenu(hWnd) != NULL);

	if (wndStyle & WS_VSCROLL) wndRect.right += GetSystemMetrics(SM_CYVSCROLL);
	if (wndStyle & WS_HSCROLL) wndRect.bottom += GetSystemMetrics(SM_CXVSCROLL);

	SetWindowPos(hWnd, NULL, 0, 0, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, SWP_NOMOVE | SWP_NOZORDER);
}
