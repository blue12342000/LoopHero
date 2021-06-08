#include "FontManager.h"

HRESULT FontManager::Init()
{
	FontInfo fontInfo;
	fontInfo.type = FONT_LOAD_TYPE::DIRECT;
	LOGFONT& logFont = fontInfo.logFont;

	logFont.lfHeight = 0;
	logFont.lfWidth = 0;
	logFont.lfWeight = 0;
	logFont.lfUnderline = 0;
	logFont.lfOrientation = 0;
	logFont.lfStrikeOut = 0;
	logFont.lfEscapement = 0;
	logFont.lfClipPrecision = 0;
	logFont.lfItalic = 0;
	logFont.lfCharSet = HANGEUL_CHARSET;
	logFont.lfQuality = DEFAULT_QUALITY;
	logFont.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy_s(logFont.lfFaceName, "µ¸¿ò");

	logFont.lfWeight = FW_BOLD;
	mFontInfos.insert(make_pair("Bµ¸¿ò", fontInfo));
	logFont.lfWeight = FW_LIGHT;
	mFontInfos.insert(make_pair("Lµ¸¿ò", fontInfo));
	logFont.lfWeight = FW_BOLD;
	logFont.lfWidth = 5;
	mFontInfos.insert(make_pair("Uµ¸¿ò", fontInfo));

	logFont.lfWidth = 0;
	strcpy_s(logFont.lfFaceName, "°íµñ");
	mFontInfos.insert(make_pair("L°íµñ", fontInfo));
	logFont.lfWeight = FW_BOLD;
	mFontInfos.insert(make_pair("B°íµñ", fontInfo));

	LoadFontFile("B³ª´®¹Ù¸¥°íµñ", "NanumBarunGothicBold");
	LoadFontFile("L³ª´®¹Ù¸¥°íµñ", "NanumBarunGothicLight");
	LoadFontFile("B³ª´®¹Ù¸¥Ææ", "NanumBarunpenB");
	LoadFontFile("L³ª´®¹Ù¸¥Ææ", "NanumBarunpenR");

	return S_OK;
}

void FontManager::Release()
{
	for (auto& pair : mFontInfos)
	{
		if (pair.second.type == FONT_LOAD_TYPE::FILE)
		{
			RemoveFontResource(pair.second.fontName.c_str());
		}
	}

	for (auto& pair : mHFonts)
	{
		DeleteObject(pair.second);
	}
	mHFonts.clear();
}

void FontManager::LoadFontFile(string key, string fontName)
{
	char currDirectory[128];
	GetCurrentDirectory(128, currDirectory);

	string filePath = string(currDirectory) + "\\Font\\" + fontName + ".ttf";
	AddFontResourceEx(filePath.c_str(), FR_PRIVATE, 0);

	FontInfo fontInfo;
	fontInfo.fontName = filePath;
	fontInfo.logFont.lfHeight = 0;
	fontInfo.logFont.lfWidth = 0;
	fontInfo.logFont.lfWeight = 0;
	fontInfo.logFont.lfUnderline = 0;
	fontInfo.logFont.lfOrientation = 0;
	fontInfo.logFont.lfStrikeOut = 0;
	fontInfo.logFont.lfEscapement = 0;
	fontInfo.logFont.lfClipPrecision = 0;
	fontInfo.logFont.lfItalic = 0;
	fontInfo.logFont.lfCharSet = HANGEUL_CHARSET;
	fontInfo.logFont.lfQuality = DEFAULT_QUALITY;
	fontInfo.logFont.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	fontInfo.logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy_s(fontInfo.logFont.lfFaceName, key.substr(1).c_str());
	mFontInfos.insert(make_pair(key, fontInfo));
}

HFONT FontManager::GetFont(string fontName, int fontSize)
{
	string hFontKey = fontName + "_" + to_string(fontSize);
	if (mHFonts.find(hFontKey) != mHFonts.end()) return mHFonts[hFontKey];
	if (mFontInfos.find(fontName) == mFontInfos.end()) return NULL;

	if (mFontInfos[fontName].type == FONT_LOAD_TYPE::DIRECT)
	{
		LOGFONT logFont = mFontInfos[fontName].logFont;
		logFont.lfHeight = fontSize;
		return (mHFonts.insert(make_pair(hFontKey, CreateFontIndirect(&logFont))).first)->second;
	}
	else
	{
		return (mHFonts.insert(make_pair(hFontKey, GetFont(mFontInfos[fontName].logFont.lfFaceName, fontSize))).first)->second;
	}
}