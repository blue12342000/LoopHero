#include "EquipItem.h"
#include <algorithm>
#include "EquipTable.h"
#include "TraitsTable.h"
#include "Unit.h"
#include "DataManager.h"
#include "ImageManager.h"
#include "Utill.h"
#include "Image.h"

void EquipItem::SetItemStatus(UNIT_STATUS status, int itemPower)
{
	switch (status)
	{
	case UNIT_STATUS::DMG:
		mStatus.insert(make_pair(UNIT_STATUS::MIN_DMG, itemPower * 0.1f - 0.5f * level));
		mStatus.insert(make_pair(UNIT_STATUS::MAX_DMG, itemPower * 0.1f + 0.5f * level));
		break;
	case UNIT_STATUS::DEF:
	case UNIT_STATUS::MAX_HP:
	case UNIT_STATUS::HP_REGEN:
	case UNIT_STATUS::VAMP:
	case UNIT_STATUS::COUNTER:
	case UNIT_STATUS::ALL_DMG:
	case UNIT_STATUS::EVASION:
	case UNIT_STATUS::SKEL_QULITY:
	case UNIT_STATUS::SKEL_POWER:
	case UNIT_STATUS::MOVE_SPEED:
	case UNIT_STATUS::ATK_SPEED:
		mStatus.insert(make_pair(status, itemPower * 0.1f));
		break;
	}
}

void EquipItem::Init()
{
	type = ITEM_TYPE::EQUIP;
	level = GameData::GetSingleton()->GetLoopLevel();
	lpItemImage = ImageManager::GetSingleton()->FindImage("item");

	pos = { 0, 0 };
}

void EquipItem::Release()
{
}

void EquipItem::Update(float deltaTime)
{
}

void EquipItem::Render(HDC hdc)
{
	if (lpItemImage)
	{
		lpRankImage->Render(hdc, (int)pos.x, (int)pos.y, { (int)rank, 0 });
		lpItemImage->Render(hdc, (int)pos.x, (int)pos.y, { no, (int)parts });
	}
}

EquipItem* EquipItem::CreateEquip(const Traits* lpTraits)
{
	EquipItem* lpEquip = new EquipItem();
	lpEquip->Init();

	lpEquip->parts = lpTraits->vEquips[rand() % lpTraits->vEquips.size()];
	lpEquip->level += rand() % 4 - 1;
	if (lpEquip->level < 1) lpEquip->level = 1;

	const EquipInfo* lpEquipInfo = GameData::GetSingleton()->GetEquipTable()->GetEquipInfo(lpEquip->parts);
	lpEquip->no = rand() % lpEquipInfo->count;
	lpEquip->name = lpEquipInfo->vName[lpEquip->no % lpEquipInfo->vName.size()];

	float itemPower = 50 * lpEquip->level;

	int statusIndex = GetRandomArgs(lpTraits->vTraits, lpEquipInfo->sMainStatus);
	if (statusIndex < 0)
	{
		lpEquip->Release();
		delete lpEquip;
		return nullptr;
	}

	vector<int> vIndex;
	int randNum = rand() % 100;
	if (randNum < 40) { /* 노말 */ }
	else if (randNum < 70)
	{
		// 레어
		vIndex = GetRandomArgs(lpTraits->vTraits, lpEquipInfo->sBonusStatus, 1);
	}
	else if (randNum < 90)
	{
		// 유니크
		vIndex = GetRandomArgs(lpTraits->vTraits, lpEquipInfo->sBonusStatus, 2);
	}
	else
	{
		// 전설
		vIndex = GetRandomArgs(lpTraits->vTraits, lpEquipInfo->sBonusStatus, 3);
	}
	
	lpEquip->rank = (ITEM_RANK)vIndex.size();
	
	if (vIndex.empty()) { lpEquip->SetItemStatus(lpTraits->vTraits[statusIndex], itemPower); }
	else
	{
		lpEquip->SetItemStatus(lpTraits->vTraits[statusIndex], itemPower * 0.8f);
		for (int i = 0; i < vIndex.size(); ++i)
		{
			lpEquip->SetItemStatus(lpTraits->vTraits[vIndex[i]], itemPower * 0.6f);
		}
	}
	lpEquip->lpRankImage = ImageManager::GetSingleton()->FindImage("item_rank");

	return lpEquip;
}
