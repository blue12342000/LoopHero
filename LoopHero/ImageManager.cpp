#include "ImageManager.h"
#include "Image.h"

HRESULT ImageManager::Init()
{
    mLpImages.insert(make_pair("ingame_backbuffer", new Image()));
    mLpImages["ingame_backbuffer"]->Init(644 * 2, 364 * 2);

    mLpImages.insert(make_pair("title_background", new Image()));
    mLpImages["title_background"]->Init("Image/UI/Title/title_background.bmp", 644 * 2, 364 * 2);

    mLpImages.insert(make_pair("InGame_BackGround", new Image()));
    mLpImages["InGame_BackGround"]->Init("Image/UI/InGame/InGame_BackGround.bmp", 644 * 2, 364 * 2);

    mLpImages.insert(make_pair("spider_icon", new Image()));
    mLpImages["spider_icon"]->Init("Image/Icon/spider_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("spider_card", new Image()));
    mLpImages["spider_card"]->Init("Image/UI/InGame/spider_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("village_icon", new Image()));
    mLpImages["village_icon"]->Init("Image/Icon/village_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("village_card", new Image()));
    mLpImages["village_card"]->Init("Image/UI/InGame/village_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("mountain_icon", new Image()));
    mLpImages["mountain_icon"]->Init("Image/Icon/mountain_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("mountain_card", new Image()));
    mLpImages["mountain_card"]->Init("Image/UI/InGame/mountain_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    return S_OK;
}

void ImageManager::Release()
{
    for (auto it = mLpImages.begin(); it != mLpImages.end(); ++it)
    {
        if ((it->second))
        {
            (it->second)->Release();
            delete (it->second);
            (it->second) = nullptr;
        }
    }
    mLpImages.clear();
}

Image* ImageManager::FindImage(string key)
{
    if (mLpImages.find(key) == mLpImages.end())
    {
        return nullptr;
    }

    return mLpImages[key];
}
