#include "EquipItem.h"

void EquipItem::Init()
{
	type = ITEM_TYPE::EQUIP;
	level = GameData::GetSingleton()->GetLoopLevel();
}

void EquipItem::Release()
{
}

void EquipItem::Update(float deltaTime)
{
}

void EquipItem::Render(HDC hdc)
{
}

EquipItem* EquipItem::CreateEquip(Traits* lpTraits)
{
	EquipItem* lpEquip = new EquipItem();
	lpEquip->Init();

	lpEquip->level += rand() % 4 - 1;
	if (lpEquip->level < 1) lpEquip->level = 1;

	int randNum = rand() % 100;
	if (randNum < 40)
	{
		// 노말
		lpEquip->rank = ITEM_RANK::NORMAL;
	}
	else if (randNum < 70)
	{
		// 레어
		lpEquip->rank = ITEM_RANK::RARE;
	}
	else if (randNum < 90)
	{
		// 유니크
		lpEquip->rank = ITEM_RANK::UNIQUE;
	}
	else
	{
		// 전설
		lpEquip->rank = ITEM_RANK::REGEND;
	}

	return lpEquip;
}
