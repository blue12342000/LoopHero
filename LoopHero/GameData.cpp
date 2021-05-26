#include "GameData.h"
#include "Deck.h"
#include "Card.h"
#include "Unit.h"
#include "EquipItem.h"
#include "Trait.h"
#include "Utill.h"

void GameData::LoadTraits()
{
	DataManager::GetSingleton()->LoadIniFile("Ini/traits.ini", "traits");

	map<string, map<string, string>> mDatas = DataManager::GetSingleton()->GetSingleton()->GetData("traits");
	vector<string> vStrKey, vKeyPair;
	for (auto group : mDatas)
	{
		Trait* lpTrait = new Trait();
		mLpTraits.insert(make_pair(group.first, lpTrait));
		lpTrait->traitId = group.first;
		for (const auto& pair : mUnitStatus)
		{
			if (group.second.find(pair.first) != group.second.end())
			{
				lpTrait->mInitStatus.insert(make_pair(mUnitStatus[pair.first], stof(group.second[pair.first])));
			}
		}

		vStrKey = StringSplit(group.second["UNIT_TRAITS"], ',');
		for (int i = 0; i < vStrKey.size(); ++i)
		{
			lpTrait->vTraits.push_back(mUnitStatus[vStrKey[i]]);
		}
		vStrKey = StringSplit(group.second["UNIT_EQUIP"], ',');
		for (int i = 0; i < vStrKey.size(); ++i)
		{
			lpTrait->vParts.push_back(mEquipParts[vStrKey[i]]);
		}
		vStrKey = StringSplit(group.second["UNIT_SLOT"], ',');
		for (int i = 0; i < vStrKey.size(); ++i)
		{
			vKeyPair = StringSplit(vStrKey[i], '|');
			lpTrait->mUnitSlot.insert(make_pair(mUnitSlot[vKeyPair[0]], mEquipParts[vKeyPair[1]]));
		}
	}
}

void GameData::LoadTiles()
{
	DataManager::GetSingleton()->LoadIniFile("Ini/tiles.ini", "tiles");

	map<string, map<string, string>> mDatas = DataManager::GetSingleton()->GetSingleton()->GetData("tiles");
	vector<string> vTileData;
	for (auto group : mDatas)
	{
		mLpTiles.insert(make_pair(group.first, new Tile()));
		mLpTiles[group.first]->mLpImage.insert(make_pair(TILE_TYPE::TILE, ImageManager::GetSingleton()->FindImage(group.first + "_tile")));
		mLpTiles[group.first]->mLpImage.insert(make_pair(TILE_TYPE::SELECT, ImageManager::GetSingleton()->FindImage(group.first + "_tile_select")));
		mLpTiles[group.first]->mLpImage.insert(make_pair(TILE_TYPE::WHITE, ImageManager::GetSingleton()->FindImage(group.first + "_tile_white")));
		mLpTiles[group.first]->id = group.first;
		mLpTiles[group.first]->name = mDatas[group.first]["name"];
		mLpTiles[group.first]->desc = mDatas[group.first]["desc"];
		mLpTiles[group.first]->vEtc = StringSplit(mDatas[group.first]["etc"], '|');
		vTileData = StringSplit(mDatas[group.first]["check_tile"], ',');
		if (!vTileData.empty())
		{
			for (int y = 0; y < 3; ++y)
			{
				for (int x = 0; x < 3; ++x)
				{
					mLpTiles[group.first]->checkTiles[y][x] = stoi(vTileData[y * 3 + x]);
				}
			}
		}
		if (mDatas[group.first].find("self_condition") == mDatas[group.first].end()) mLpTiles[group.first]->selfCondition = -1;
		else mLpTiles[group.first]->selfCondition = stod(mDatas[group.first]["self_condition"]);
		if (mDatas[group.first].find("near_condition") == mDatas[group.first].end()) mLpTiles[group.first]->nearCondition = -1;
		else mLpTiles[group.first]->nearCondition = stod(mDatas[group.first]["near_condition"]);
		mLpTiles[group.first]->vSelfTiles = StringSplit(mDatas[group.first]["self"], ',');
		mLpTiles[group.first]->vNearTiles = StringSplit(mDatas[group.first]["near"], ',');
	}
}

void GameData::LoadEquipInfo()
{
	DataManager::GetSingleton()->LoadIniFile("Ini/equip.ini", "equip");
	vector<string> vStrData;
	auto equip = DataManager::GetSingleton()->GetData("equip");
	for (auto& group : equip)
	{
		EquipInfo equipInfo;
		equipInfo.count = stoi(group.second["count"]);
		equipInfo.vName = StringSplit(group.second["name"], ',');

		vStrData = StringSplit(group.second["main_status"], ',');
		for (int i = 0; i < vStrData.size(); ++i)
		{
			equipInfo.sMainStatus.insert(mUnitStatus[vStrData[i]]);
		}
		vStrData = StringSplit(group.second["bonus_status"], ',');
		for (int i = 0; i < vStrData.size(); ++i)
		{
			equipInfo.sBonusStatus.insert(mUnitStatus[vStrData[i]]);
		}
		mEquipInfo.insert(make_pair(mEquipParts[group.first], equipInfo));
	}
}

HRESULT GameData::Init()
{
    lpDeck = new Deck();
    lpDeck->Init();

	string key, lang;
	for (int i = 0; i < (int)UNIT_SLOT::NONE; ++i)
	{
		switch ((UNIT_SLOT)i)
		{
		case UNIT_SLOT::LEFT_HAND:
			key = "LEFT_HAND";
			lang = "¿Þ¼Õ";
			break;
		case UNIT_SLOT::RIGHT_HAND:
			key = "RIGHT_HAND";
			lang = "¿À¸¥¼Õ";
			break;
		case UNIT_SLOT::HELMET:
			key = "HELMET";
			lang = "Çï¸Ë";
			break;
		case UNIT_SLOT::ARMOR:
			key = "ARMOR";
			lang = "°©¿Ê";
			break;
		case UNIT_SLOT::BELTS:
			key = "BELTS";
			lang = "¹ëÆ®";
			break;
		case UNIT_SLOT::GLOVES:
			key = "GLOVES";
			lang = "Àå°©";
			break;
		case UNIT_SLOT::BOOTS:
			key = "BOOTS";
			lang = "ºÎÃ÷";
			break;
		case UNIT_SLOT::LEFT_RING:
			key = "LEFT_RING";
			lang = "¿ÞÂÊ¹ÝÁö";
			break;
		case UNIT_SLOT::RIGHT_RING:
			key = "RIGHT_RING";
			lang = "¿À¸¥ÂÊ¹ÝÁö";
			break;
		case UNIT_SLOT::ARMULET:
			key = "ARMULET";
			lang = "¸ñ°ÉÀÌ";
			break;
		}
		mUnitSlotLang.insert(make_pair((UNIT_SLOT)i, lang));
		mUnitSlot.insert(make_pair(key, (UNIT_SLOT)i));
	}
	
	for (int i = 0; i < (int)UNIT_STATUS::NONE; ++i)
	{
		switch ((UNIT_STATUS)i)
		{
		case UNIT_STATUS::DMG:
			key = "DMG";
			lang = "°ø°Ý·Â";
			break;
		case UNIT_STATUS::MIN_DMG:
			key = "MIN_DMG";
			lang = "ÃÖ¼Ò °ø°Ý·Â";
			break;
		case UNIT_STATUS::MAX_DMG:
			key = "MAX_DMG";
			lang = "ÃÖ´ë °ø°Ý·Â";
			break;
		case UNIT_STATUS::DEF:
			key = "DEF";
			lang = "¹æ¾î·Â";
			break;
		case UNIT_STATUS::MAX_HP:
			key = "MAX_HP";
			lang = "ÃÖ´ëÃ¼·Â";
			break;
		case UNIT_STATUS::HP_REGEN:
			key = "HP_REGEN";
			lang = "ÃÊ´çÈ¸º¹";
			break;
		case UNIT_STATUS::VAMP:
			key = "VAMP";
			lang = "ÈíÇ÷";
			break;
		case UNIT_STATUS::COUNTER:
			key = "COUNTER";
			lang = "¹Ý°ÝÈ®·ü";
			break;
		case UNIT_STATUS::ALL_DMG:
			key = "ALL_DMG";
			lang = "ÀüÃ¼ °ø°Ý·Â";
			break;
		case UNIT_STATUS::EVASION:
			key = "EVASION";
			lang = "È¸ÇÇÀ²";
			break;
		case UNIT_STATUS::SKEL_QULITY:
			key = "SKEL_QULITY";
			lang = "ÇØ°ñÀÇ Áú";
			break;
		case UNIT_STATUS::SKEL_POWER:
			key = "SKEL_POWER";
			lang = "ÇØ°ñ ÆÄ¿ö";
			break;
		case UNIT_STATUS::MOVE_SPEED:
			key = "MOVE_SPEED";
			lang = "ÀÌµ¿¼Óµµ";
			break;
		case UNIT_STATUS::ATK_SPEED:
			key = "ATK_SPEED";
			lang = "°ø°Ý¼Óµµ";
			break;
		}
		mUnitStatusLang.insert(make_pair((UNIT_STATUS)i, lang));
		mUnitStatus.insert(make_pair(key, (UNIT_STATUS)i));
	}

	for (int i = 0; i < (int)EQUIP_PARTS::NONE; ++i)
	{
		switch ((EQUIP_PARTS)i)
		{
		case EQUIP_PARTS::SWORD:
			key = "SWORD";
			lang = "°Ë";
			break;
		case EQUIP_PARTS::BOOK:
			key = "BOOK";
			lang = "¸¶µµ¼­";
			break;
		case EQUIP_PARTS::SHIELD:
			key = "SHIELD";
			lang = "¹æÆÐ";
			break;
		case EQUIP_PARTS::HELMET:
			key = "HELMET";
			lang = "Çï¸Ë";
			break;
		case EQUIP_PARTS::ARMOR:
			key = "ARMOR";
			lang = "°©¿Ê";
			break;
		case EQUIP_PARTS::BELTS:
			key = "BELTS";
			lang = "¹ëÆ®";
			break;
		case EQUIP_PARTS::GLOVES:
			key = "GLOVES";
			lang = "Àå°©";
			break;
		case EQUIP_PARTS::BOOTS:
			key = "BOOTS";
			lang = "ºÎÃ÷";
			break;
		case EQUIP_PARTS::RING:
			key = "RING";
			lang = "¹ÝÁö";
			break;
		case EQUIP_PARTS::ARMULET:
			key = "ARMULET";
			lang = "¸ñ°ÉÀÌ";
			break;
		}
		mEquipPartsLang.insert(make_pair((EQUIP_PARTS)i, lang));
		mEquipParts.insert(make_pair(key, (EQUIP_PARTS)i));
	}

	LoadTraits();
	LoadTiles();
	LoadEquipInfo();

    return S_OK;
}

void GameData::Release()
{
    if (lpDeck)
    {
        lpDeck->Release();
        delete lpDeck;
        lpDeck = nullptr;
    }

	for (auto& pair : mLpTraits)
	{
		delete pair.second;
	}
	mLpTraits.clear();

	for (auto& pair : mLpTiles)
	{
		delete pair.second;
	}
	mLpTiles.clear();
}

Card* GameData::PickCard()
{
    return lpDeck->GetRandomCard();
}

const EquipInfo* GameData::GetEquipInfo(EQUIP_PARTS parts)
{
	auto it = mEquipInfo.find(parts);
	if (it == mEquipInfo.end()) return nullptr;

	return &(it->second);
}

Trait* GameData::GetTrait(string traitKey)
{
	const auto& it = mLpTraits.find(traitKey);
	if (it == mLpTraits.end())
	{
		return nullptr;
	}

	return it->second;
}

string GameData::GetLang(UNIT_SLOT slot)
{
	if (mUnitSlotLang.find(slot) == mUnitSlotLang.end()) return "";

	return mUnitSlotLang[slot];
}

string GameData::GetLang(UNIT_STATUS status)
{
	if (mUnitStatusLang.find(status) == mUnitStatusLang.end()) return "";

	return mUnitStatusLang[status];
}

string GameData::GetLang(EQUIP_PARTS parts)
{
	if (mEquipPartsLang.find(parts) == mEquipPartsLang.end()) return "";

	return mEquipPartsLang[parts];
}
