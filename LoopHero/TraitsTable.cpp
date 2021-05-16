#include "TraitsTable.h"
#include "LoopHero.h"
#include "Utill.h"
#include "Unit.h"
#include "EquipItem.h"

void TraitsTable::LoadTraitsData()
{
	DataManager::GetSingleton()->LoadIniFile("Ini/traits.ini", "traits");


	string slotKey;
	map<string, UNIT_SLOT> mUnitSlot;
	for (int i = 0; i < (int)UNIT_SLOT::NONE; ++i)
	{
		switch ((UNIT_SLOT)i)
		{
		case UNIT_SLOT::LEFT_HAND:
			slotKey = "LEFT_HAND";
			break;
		case UNIT_SLOT::RIGHT_HAND:
			slotKey = "RIGHT_HAND";
			break;
		case UNIT_SLOT::HELMET:
			slotKey = "HELMET";
			break;
		case UNIT_SLOT::ARMOR:
			slotKey = "ARMOR";
			break;
		case UNIT_SLOT::BELTS:
			slotKey = "BELTS";
			break;
		case UNIT_SLOT::BOOTS:
			slotKey = "BOOTS";
			break;
		case UNIT_SLOT::LEFT_RING:
			slotKey = "LEFT_RING";
			break;
		case UNIT_SLOT::RIGHT_RING:
			slotKey = "RIGHT_RING";
			break;
		case UNIT_SLOT::ARMULET:
			slotKey = "ARMULET";
			break;
		}
		mUnitSlot.insert(make_pair(slotKey, (UNIT_SLOT)i));
	}

	string statusKey;
	map<string, UNIT_STATUS> mUnitStatus;
	for (int i = 0; i < (int)UNIT_STATUS::NONE; ++i)
	{
		switch ((UNIT_STATUS)i)
		{
		case UNIT_STATUS::MIN_DMG:
			statusKey = "MIN_DMG";
			break;
		case UNIT_STATUS::MAX_DMG:
			statusKey = "MAX_DMG";
			break;
		case UNIT_STATUS::DEF:
			statusKey = "DEF";
			break;
		case UNIT_STATUS::MAX_HP:
			statusKey = "MAX_HP";
			break;
		case UNIT_STATUS::HP_REGEN:
			statusKey = "HP_REGEN";
			break;
		case UNIT_STATUS::VAMP:
			statusKey = "VAMP";
			break;
		case UNIT_STATUS::COUNTER:
			statusKey = "COUNTER";
			break;
		case UNIT_STATUS::ALL_DMG:
			statusKey = "ALL_DMG";
			break;
		case UNIT_STATUS::EVASION:
			statusKey = "EVASION";
			break;
		case UNIT_STATUS::SKEL_QULITY:
			statusKey = "SKEL_QULITY";
			break;
		case UNIT_STATUS::SKEL_POWER:
			statusKey = "SKEL_POWER";
			break;
		case UNIT_STATUS::MOVE_SPEED:
			statusKey = "MOVE_SPEED";
			break;
		case UNIT_STATUS::ATK_SPEED:
			statusKey = "ATK_SPEED";
			break;
		}
		mUnitStatus.insert(make_pair(statusKey, (UNIT_STATUS)i));
	}

	string equipKey;
	map<string, EQUIP_PARTS> mUnitEquip;
	for (int i = 0; i < (int)EQUIP_PARTS::NONE; ++i)
	{
		switch ((EQUIP_PARTS)i)
		{
		case EQUIP_PARTS::SWORD:
			equipKey = "SWORD";
			break;
		case EQUIP_PARTS::BOOK:
			equipKey = "BOOK";
			break;
		case EQUIP_PARTS::SHIELD:
			equipKey = "SHIELD";
			break;
		case EQUIP_PARTS::HELMET:
			equipKey = "HELMET";
			break;
		case EQUIP_PARTS::ARMOR:
			equipKey = "ARMOR";
			break;
		case EQUIP_PARTS::BELTS:
			equipKey = "BELTS";
			break;
		case EQUIP_PARTS::GLOVES:
			equipKey = "GLOVES";
			break;
		case EQUIP_PARTS::BOOTS:
			equipKey = "BOOTS";
			break;
		case EQUIP_PARTS::RING:
			equipKey = "RING";
			break;
		case EQUIP_PARTS::ARMULET:
			equipKey = "ARMULET";
			break;
		}
		mUnitEquip.insert(make_pair(equipKey, (EQUIP_PARTS)i));
	}

	map<string, map<string, string>> mDatas = DataManager::GetSingleton()->GetSingleton()->GetData("traits");
	vector<string> vStrKey, vKeyPair;
	for (auto group : mDatas)
	{
		Traits* lpTraits = new Traits();
		mLpTraits.insert(make_pair(group.first, lpTraits));
		lpTraits->traitId = group.first;
		for (const auto& pair : mUnitStatus)
		{
			if (group.second.find(pair.first) != group.second.end())
			{
				lpTraits->mInitStatus.insert(make_pair(mUnitStatus[pair.first], stof(group.second[pair.first])));
			}
		}

		vStrKey = StringSplit(group.second["UNIT_TRAITS"], ',');
		for (int i = 0; i < vStrKey.size(); ++i)
		{
			lpTraits->vTraits.push_back(mUnitStatus[vStrKey[i]]);
		}
		vStrKey = StringSplit(group.second["UNIT_EQUIP"], ',');
		for (int i = 0; i < vStrKey.size(); ++i)
		{
			lpTraits->vEquips.push_back(mUnitEquip[vStrKey[i]]);
		}
		vStrKey = StringSplit(group.second["UNIT_SLOT"], ',');
		for (int i = 0; i < vStrKey.size(); ++i)
		{
			vKeyPair = StringSplit(vStrKey[i], '|');
			lpTraits->mUnitEquip.insert(make_pair(mUnitSlot[vKeyPair[0]], mUnitEquip[vKeyPair[1]]));
		}
	}

	int a = 0;
}

Unit* TraitsTable::CreateUnit(string traitKey)
{
	auto it = mLpTraits.find(traitKey);
	if (it == mLpTraits.end()) return nullptr;

	Unit* lpUnit = new Unit();
	lpUnit->lpTraits = it->second;
	lpUnit->Init();

	return lpUnit;
}
