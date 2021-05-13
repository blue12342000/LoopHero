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

    mLpImages.insert(make_pair("ingame_bosstimer_background", new Image()));
    mLpImages["ingame_bosstimer_background"]->Init("Image/UI/InGame/ingame_bosstimer_background.bmp", 121 * 2, 27 * 2);

    mLpImages.insert(make_pair("ingame_info_background", new Image()));
    mLpImages["ingame_info_background"]->Init("Image/UI/InGame/ingame_info_background.bmp", 300 * 2, 364 * 2, 2, 1, 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("spider_icon", new Image()));
    mLpImages["spider_icon"]->Init("Image/Icon/spider_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("spider_card", new Image()));
    mLpImages["spider_card"]->Init("Image/Cards/spider_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("village_icon", new Image()));
    mLpImages["village_icon"]->Init("Image/Icon/village_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("village_card", new Image()));
    mLpImages["village_card"]->Init("Image/Cards/village_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("mountain_icon", new Image()));
    mLpImages["mountain_icon"]->Init("Image/Icon/mountain_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("mountain_card", new Image()));
    mLpImages["mountain_card"]->Init("Image/Cards/mountain_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("battlefield_icon", new Image()));
    mLpImages["battlefield_icon"]->Init("Image/Icon/battlefield_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("battlefield_card", new Image()));
    mLpImages["battlefield_card"]->Init("Image/Cards/battlefield_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("bloodgrove_icon", new Image()));
    mLpImages["bloodgrove_icon"]->Init("Image/Icon/bloodgrove_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("bloodgrove_card", new Image()));
    mLpImages["bloodgrove_card"]->Init("Image/Cards/bloodgrove_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("cemetery_icon", new Image()));
    mLpImages["cemetery_icon"]->Init("Image/Icon/cemetery_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("cemetery_card", new Image()));
    mLpImages["cemetery_card"]->Init("Image/Cards/cemetery_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("crystal_icon", new Image()));
    mLpImages["crystal_icon"]->Init("Image/Icon/crystal_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("crystal_card", new Image()));
    mLpImages["crystal_card"]->Init("Image/Cards/crystal_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("grove_icon", new Image()));
    mLpImages["grove_icon"]->Init("Image/Icon/grove_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("grove_card", new Image()));
    mLpImages["grove_card"]->Init("Image/Cards/grove_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("lantern_icon", new Image()));
    mLpImages["lantern_icon"]->Init("Image/Icon/lantern_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("lantern_card", new Image()));
    mLpImages["lantern_card"]->Init("Image/Cards/lantern_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("meadom_icon", new Image()));
    mLpImages["meadom_icon"]->Init("Image/Icon/meadom_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("meadom_card", new Image()));
    mLpImages["meadom_card"]->Init("Image/Cards/meadom_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("swamp_icon", new Image()));
    mLpImages["swamp_icon"]->Init("Image/Icon/swamp_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("swamp_card", new Image()));
    mLpImages["swamp_card"]->Init("Image/Cards/swamp_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("oblivion_icon", new Image()));
    mLpImages["oblivion_icon"]->Init("Image/Icon/oblivion_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("oblivion_card", new Image()));
    mLpImages["oblivion_card"]->Init("Image/Cards/oblivion_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("rock_icon", new Image()));
    mLpImages["rock_icon"]->Init("Image/Icon/rock_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("rock_card", new Image()));
    mLpImages["rock_card"]->Init("Image/Cards/rock_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

    mLpImages.insert(make_pair("tresury_icon", new Image()));
    mLpImages["tresury_icon"]->Init("Image/Icon/tresury_icon.bmp", 29 * 2, 29 * 2);
    mLpImages.insert(make_pair("tresury_card", new Image()));
    mLpImages["tresury_card"]->Init("Image/Cards/tresury_card.bmp", 41 * 2, 58 * 2, true, RGB(0, 170, 255));

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
