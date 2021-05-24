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

	string key;
	for (int i = 0; i < (int)UNIT_SLOT::NONE; ++i)
	{
		switch ((UNIT_SLOT)i)
		{
		case UNIT_SLOT::LEFT_HAND:
			key = "LEFT_HAND";
			break;
		case UNIT_SLOT::RIGHT_HAND:
			key = "RIGHT_HAND";
			break;
		case UNIT_SLOT::HELMET:
			key = "HELMET";
			break;
		case UNIT_SLOT::ARMOR:
			key = "ARMOR";
			break;
		case UNIT_SLOT::BELTS:
			key = "BELTS";
			break;
		case UNIT_SLOT::GLOVES:
			key = "GLOVES";
			break;
		case UNIT_SLOT::BOOTS:
			key = "BOOTS";
			break;
		case UNIT_SLOT::LEFT_RING:
			key = "LEFT_RING";
			break;
		case UNIT_SLOT::RIGHT_RING:
			key = "RIGHT_RING";
			break;
		case UNIT_SLOT::ARMULET:
			key = "ARMULET";
			break;
		}
		mUnitSlot.insert(make_pair(key, (UNIT_SLOT)i));
	}
	
	for (int i = 0; i < (int)UNIT_STATUS::NONE; ++i)
	{
		switch ((UNIT_STATUS)i)
		{
		case UNIT_STATUS::MIN_DMG:
			key = "MIN_DMG";
			break;
		case UNIT_STATUS::MAX_DMG:
			key = "MAX_DMG";
			break;
		case UNIT_STATUS::DEF:
			key = "DEF";
			break;
		case UNIT_STATUS::MAX_HP:
			key = "MAX_HP";
			break;
		case UNIT_STATUS::HP_REGEN:
			key = "HP_REGEN";
			break;
		case UNIT_STATUS::VAMP:
			key = "VAMP";
			break;
		case UNIT_STATUS::COUNTER:
			key = "COUNTER";
			break;
		case UNIT_STATUS::ALL_DMG:
			key = "ALL_DMG";
			break;
		case UNIT_STATUS::EVASION:
			key = "EVASION";
			break;
		case UNIT_STATUS::SKEL_QULITY:
			key = "SKEL_QULITY";
			break;
		case UNIT_STATUS::SKEL_POWER:
			key = "SKEL_POWER";
			break;
		case UNIT_STATUS::MOVE_SPEED:
			key = "MOVE_SPEED";
			break;
		case UNIT_STATUS::ATK_SPEED:
			key = "ATK_SPEED";
			break;
		}
		mUnitStatus.insert(make_pair(key, (UNIT_STATUS)i));
	}

	for (int i = 0; i < (int)EQUIP_PARTS::NONE; ++i)
	{
		switch ((EQUIP_PARTS)i)
		{
		case EQUIP_PARTS::SWORD:
			key = "SWORD";
			break;
		case EQUIP_PARTS::BOOK:
			key = "BOOK";
			break;
		case EQUIP_PARTS::SHIELD:
			key = "SHIELD";
			break;
		case EQUIP_PARTS::HELMET:
			key = "HELMET";
			break;
		case EQUIP_PARTS::ARMOR:
			key = "ARMOR";
			break;
		case EQUIP_PARTS::BELTS:
			key = "BELTS";
			break;
		case EQUIP_PARTS::GLOVES:
			key = "GLOVES";
			break;
		case EQUIP_PARTS::BOOTS:
			key = "BOOTS";
			break;
		case EQUIP_PARTS::RING:
			key = "RING";
			break;
		case EQUIP_PARTS::ARMULET:
			key = "ARMULET";
			break;
		}
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
