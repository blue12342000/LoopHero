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
	strcpy_s(logFont.lfFaceName, "����");

	logFont.lfWeight = FW_BOLD;
	mLogFonts.insert(make_pair("B����", logFont));
	logFont.lfWeight = FW_LIGHT;
	mLogFonts.insert(make_pair("L����", logFont));
	logFont.lfWeight = FW_BOLD;
	logFont.lfWidth = 5;
	mLogFonts.insert(make_pair("U����", logFont));

	logFont.lfWidth = 0;
	strcpy_s(logFont.lfFaceName, "���");
	mLogFonts.insert(make_pair("L���", logFont));
	logFont.lfWeight = FW_BOLD;
	mLogFonts.insert(make_pair("B���", logFont));

	strcpy_s(logFont.lfFaceName, "�����ٸ����");
	mLogFonts.insert(make_pair("B�����ٸ����", logFont));

	logFont.lfWeight = FW_LIGHT;
	mLogFonts.insert(make_pair("L�����ٸ����", logFont));

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