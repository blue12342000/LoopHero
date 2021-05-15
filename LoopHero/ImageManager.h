#pragma once
#include "LoopHero.h"
#include "Singleton.h"

class Image;
class ImageManager : public Singleton<ImageManager>
{
private:
	map<string, Image*> mLpImages;

public:
	HRESULT Init();
	void Release();

	Image* AddImage(string key, int width, int height, bool isTrans = false, COLORREF transColor = NULL);

	Image* FindImage(string key);
};

