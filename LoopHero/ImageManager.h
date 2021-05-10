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

	Image* FindImage(string key);
};

