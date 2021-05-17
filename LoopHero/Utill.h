#pragma once
#include <string>
#include <vector>
#include <set>
#include <Windows.h>

using namespace std;

double StackCalculate(string text);
vector<string> StringSplit(string str, char token);

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