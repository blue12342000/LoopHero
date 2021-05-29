#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <string>
#include <map>

#define FONT_SIZE_PT 1.7635f

using namespace std;

class FontManager : public Singleton<FontManager>
{
private:
	map<string, LOGFONT> mLogFonts;
	map<string, HFONT> mHFonts;

public:
	HRESULT Init();
	void Release();

	HFONT GetFont(string fontName, int fontSize);
};

