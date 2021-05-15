#pragma once
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

double StackCalculate(string text);
vector<string> StringSplit(string str, char token);

void RenderRectangle(HDC hdc, RECT rc, COLORREF color = RGB(255, 255, 255));