#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <string>
#include <map>

using namespace std;

enum class FONT_LOAD_TYPE
{
	DIRECT,
	FILE
};

struct FontInfo
{
	FONT_LOAD_TYPE type;
	string fontName;
	LOGFONT logFont;
};

class FontManager : public Singleton<FontManager>
{
private:
	map<string, FontInfo> mFontInfos;
	map<string, HFONT> mHFonts;

public:
	HRESULT Init();
	void Release();

	void LoadFontFile(string key, string fontName);
	HFONT GetFont(string fontName, int fontSize);
};

