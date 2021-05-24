#include "Trait.h"
#include "Unit.h"
#include "EquipItem.h"
#include "GameData.h"
#include "Utill.h"

Unit* Trait::NewUnit(string trait)
{
    Unit* lpUnit = GameObject::Create<Unit>();
    Trait* lpTrait = GameData::GetSingleton()->GetTrait(trait);
    lpUnit->Init();
    if (lpTrait) lpUnit->SetTrait(*lpTrait);
    return lpUnit;
}

EquipItem* Trait::CreateEquip()
{
	EquipItem* lpEquip = GameObject::Create<EquipItem>();
	lpEquip->Init();

	lpEquip->parts = vParts[rand() % vParts.size()];
	lpEquip->level += rand() % 4 - 1;
	if (lpEquip->level < 1) lpEquip->level = 1;

	const EquipInfo* lpEquipInfo = GameData::GetSingleton()->GetEquipInfo(lpEquip->parts);
	lpEquip->no = rand() % lpEquipInfo->count;
	lpEquip->name = lpEquipInfo->vName[lpEquip->no % lpEquipInfo->vName.size()];

	float itemPower = 50 * lpEquip->level;

	int statusIndex = GetRandomArgs(vTraits, lpEquipInfo->sMainStatus);
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
		vIndex = GetRandomArgs(vTraits, lpEquipInfo->sBonusStatus, 1);
	}
	else if (randNum < 90)
	{
		// 유니크
		vIndex = GetRandomArgs(vTraits, lpEquipInfo->sBonusStatus, 2);
	}
	else
	{
		// 전설
		vIndex = GetRandomArgs(vTraits, lpEquipInfo->sBonusStatus, 3);
	}

	lpEquip->rank = (ITEM_RANK)vIndex.size();

	if (vIndex.empty()) { lpEquip->SetItemStatus(vTraits[statusIndex], itemPower); }
	else
	{
		lpEquip->SetItemStatus(vTraits[statusIndex], itemPower * 0.8f);
		for (int i = 0; i < vIndex.size(); ++i)
		{
			lpEquip->SetItemStatus(vTraits[vIndex[i]], itemPower * 0.6f);
		}
	}
	lpEquip->lpRankImage = ImageManager::GetSingleton()->FindImage("item_rank");

	return lpEquip;
}
