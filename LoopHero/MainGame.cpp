#include "MainGame.h"
#include "InGame.h"
#include "Timer.h"

double StackCalculate(string text)
{
	bool isNextNum = true;
	double ret = 0;
	vector<string> pfx;
	pfx.reserve(text.length());
	vector<string> cal;
	cal.reserve(text.length());
	for (int i = 0; i < text.length(); ++i)
	{
		switch (text[i])
		{
		case '*': case '%': case '-': case '+': case '(':
			while (!pfx.empty() && (pfx.back()[0] == '*' || pfx.back()[0] == '/'))
			{
				cal.push_back(pfx.back());
				pfx.pop_back();
			}
			if (cal.size() == 0 && text[i] == '-')
			{
				cal.push_back(string{ text[i] });
				isNextNum = false;
			}
			else
			{
				pfx.push_back(string{ text[i] });
				isNextNum = true;
			}
			break;
		case '.': case '0': case '1': case '2':
		case '3': case '4': case '5': case '7':
		case '8': case '9':
			if (isNextNum) cal.push_back("");
			cal.back().push_back(text[i]);
			isNextNum = false;
			break;
		case ')':
			while (pfx.back()[0] != '(')
			{
				cal.push_back(pfx.back());
				pfx.pop_back();
			}
			pfx.pop_back();
			break;
		}
	}
	if (!pfx.empty()) cal.insert(cal.end(), pfx.crbegin(), pfx.crend());

	for (int i = 0; i < cal.size();)
	{
		if (cal[i].length() > 1)
		{
			++i;
			continue;
		}

		switch (cal[i][0])
		{
		case '-':
			cal[i] = to_string(stod(cal[i - 2]) - stod(cal[i - 1]));
			cal.erase(cal.begin() + i - 2, cal.begin() + i);
			--i;
			break;
		case '+':
			cal[i] = to_string(stod(cal[i - 2]) + stod(cal[i - 1]));
			cal.erase(cal.begin() + i - 2, cal.begin() + i);
			--i;
			break;
		case '*':
			cal[i] = to_string(stod(cal[i - 2]) * stod(cal[i - 1]));
			cal.erase(cal.begin() + i - 2, cal.begin() + i);
			--i;
			break;
		case '/':
			cal[i] = to_string(stod(cal[i - 2]) / stod(cal[i - 1]));
			cal.erase(cal.begin() + i - 2, cal.begin() + i);
			--i;
			break;
		default:
			++i;
			break;
		}
	}
	ret = stod(cal[0]);
	return ret;
}

HRESULT MainGame::Init()
{
	hdc = GetDC(g_hWnd);
	KeyManager::GetSingleton()->Init();
	ImageManager::GetSingleton()->Init();
	SceneManager::GetSingleton()->Init();

	DataManager::GetSingleton()->LoadIniFile("Ini/lang_kor2.ini");

	double a = StackCalculate("-5.3*5+(18-10)+3");

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
