#include "FontManager.h"

HRESULT FontManager::Init()
{
	LOGFONT logFont;
	logFont.lfHeight = FONT_SIZE_PT;
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
	mLogFonts.insert(make_pair("Bµ¸¿ò", logFont));
	logFont.lfWeight = FW_LIGHT;
	mLogFonts.insert(make_pair("Lµ¸¿ò", logFont));
	logFont.lfWeight = FW_BOLD;
	logFont.lfWidth = 5;
	mLogFonts.insert(make_pair("Uµ¸¿ò", logFont));

	logFont.lfWidth = 0;
	strcpy_s(logFont.lfFaceName, "°íµñ");
	mLogFonts.insert(make_pair("L°íµñ", logFont));
	logFont.lfWeight = FW_BOLD;
	mLogFonts.insert(make_pair("B°íµñ", logFont));

	strcpy_s(logFont.lfFaceName, "³ª´®¹Ù¸¥°íµñ");
	mLogFonts.insert(make_pair("B³ª´®¹Ù¸¥°íµñ", logFont));

	logFont.lfWeight = FW_LIGHT;
	mLogFonts.insert(make_pair("L³ª´®¹Ù¸¥°íµñ", logFont));

	return S_OK;
}

void FontManager::Release()
{
	for (auto& pair : mHFonts)
	{
		DeleteObject(pair.second);
	}
	mHFonts.clear();
}

HFONT FontManager::GetFont(string fontName, int fontSize)
{
	string hFontKey = fontName + "_" + to_string(fontSize);
	if (mHFonts.find(hFontKey) != mHFonts.end()) return mHFonts[hFontKey];
	if (mLogFonts.find(fontName) == mLogFonts.end()) return NULL;

	LOGFONT logFont = mLogFonts[fontName];
	logFont.lfHeight *= fontSize;
	return (mHFonts.insert(make_pair(hFontKey, CreateFontIndirect(&logFont))).first)->second;
}