#include "Utill.h"

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
		case '*': case '/': case '-': case '+': case '(':
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

vector<string> StringSplit(string str, char token)
{
	vector<string> vString;
	int offset = 0, preOffset = 0;
	while ((offset = str.find(token, preOffset)) != -1)
	{
		vString.push_back(str.substr(preOffset, offset - preOffset));
		preOffset = offset + 1;
	}
	if (!str.empty()) vString.push_back(str.substr(preOffset, str.size() - preOffset));
	return vString;
}

void RenderRectangle(HDC hdc, RECT rc, COLORREF color)
{
	HBRUSH hBrudh = CreateSolidBrush(color), hOldBrush;
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrudh);
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
	DeleteObject(SelectObject(hdc, hOldBrush));
}
