#include "EquipTable.h"
#include "EquipItem.h"
#include "Unit.h"
#include "DataManager.h"
#include "Utill.h"

const EquipInfo* EquipTable::GetEquipInfo(EQUIP_PARTS parts)
{
	auto it = mEquipInfo.find(parts);
	if (it == mEquipInfo.end()) return nullptr;

    return &(it->second);
}

void EquipTable::LoadEquipData()
{
    DataManager::GetSingleton()->LoadIniFile("Ini/equip.ini", "equip");

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
			equipInfo.vMainStatus.push_back(mUnitStatus[vStrData[i]]);
		}
		vStrData = StringSplit(group.second["bonus_status"], ',');
		for (int i = 0; i < vStrData.size(); ++i)
		{
			equipInfo.vBonusStatus.push_back(mUnitStatus[vStrData[i]]);
		}
		mEquipInfo.insert(make_pair(mUnitEquip[group.first], equipInfo));
	}
}
