#pragma once
#include <string>
#include <vector>
#include <set>
#include <Windows.h>
#include <algorithm>

using namespace std;

inline void SetWindowSize(HWND hWnd, int width, int height)
{
	RECT wndRect;
	DWORD wndStyle = GetWindowLong(hWnd, GWL_STYLE);

	SetRect(&wndRect, 0, 0, width, height);
	AdjustWindowRect(&wndRect, wndStyle, GetMenu(hWnd) != NULL);

	if (wndStyle & WS_VSCROLL) wndRect.right += GetSystemMetrics(SM_CYVSCROLL);
	if (wndStyle & WS_HSCROLL) wndRect.bottom += GetSystemMetrics(SM_CXVSCROLL);

	SetWindowPos(hWnd, NULL, 0, 0, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, SWP_NOMOVE | SWP_NOZORDER);
}

double StackCalculate(string text);
vector<string> StringSplit(string str, char token);
RECT MakeRect(POINTFLOAT pos, int width, int height);
void RenderRectangle(HDC hdc, RECT rc, COLORREF color = RGB(255, 255, 255));

template <typename T>
inline int GetRandomArgs(const vector<T>& args, const set<T>& in)
{
	vector<int> vIndex;
	vIndex.reserve(args.size());
	for (int i = 0; i < args.size(); ++i)
	{
		if (in.find(args[i]) != in.end()) vIndex.push_back(i);
	}
	if (!vIndex.empty()) return vIndex[rand() % vIndex.size()];
	return -1;
}

template <typename T>
inline vector<int> GetRandomArgs(const vector<T>& args, const set<T>& in, int count)
{
	vector<int> vIndex;
	if (count < 1) return vIndex;

	vIndex.reserve(args.size());
	for (int i = 0; i < args.size(); ++i)
	{
		if (in.find(args[i]) != in.end()) vIndex.push_back(i);
	}
	while (vIndex.size() > count)
	{
		vIndex.erase(vIndex.begin() + rand() % vIndex.size());
	}

	return vIndex;
}

template<typename T>
inline T LerpAxis(T from, T to, float ratio)
{
	if (ratio < 1)
	{
		return from + (T)((to - from) * ratio);
	}
	else
	{
		return to;
	}
}

template<>
inline POINT LerpAxis(POINT from, POINT to, float ratio)
{
	if (ratio < 1)
	{
		return POINT{ LerpAxis(from.x, to.x, ratio), LerpAxis(from.y, to.y, ratio) };
	}
	else
	{
		return to;
	}
}

template<>
inline POINTFLOAT LerpAxis(POINTFLOAT from, POINTFLOAT to, float ratio)
{
	if (ratio < 1)
	{
		return POINTFLOAT{ LerpAxis(from.x, to.x, ratio), LerpAxis(from.y, to.y, ratio) };
	}
	else
	{
		return to;
	}
}

template<typename T>
inline T LerpAxis(vector<T>& vAxis, float ratio)
{
	if (vAxis.empty()) return T();

	if (ratio < 1)
	{
		vector<T> vLerpAxis;
		vLerpAxis.reserve(vAxis.size());
		for (int i = 0, n = vAxis.size() - 1; i < n; ++i)
		{
			vLerpAxis.push_back(LerpAxis(vAxis[i], vAxis[i + 1], ratio));
		}
		if (vLerpAxis.size() == 1) return vLerpAxis.back();
		else return LerpAxis(vLerpAxis, ratio);
	}
	else
	{
		return vAxis.back();
	}
}

inline POINTFLOAT operator+(POINTFLOAT& a, POINTFLOAT& b)
{
	return POINTFLOAT{ a.x + b.x, a.y + b.y };
}

inline POINTFLOAT operator+(POINTFLOAT a, POINTFLOAT b)
{
	return POINTFLOAT{ a.x + b.x, a.y + b.y };
}