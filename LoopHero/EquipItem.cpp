#include "EquipItem.h"
#include <algorithm>
#include "Trait.h"
#include "Unit.h"
#include "DataManager.h"
#include "ImageManager.h"
#include "Utill.h"
#include "Image.h"
#include "Text.h"

void EquipItem::SetItemStatus(UNIT_STATUS status, int itemPower)
{
	switch (status)
	{
	case UNIT_STATUS::DMG:
		mStatus.insert(make_pair(UNIT_STATUS::MIN_DMG, itemPower * 0.1f - 0.5f * level));
		mStatus.insert(make_pair(UNIT_STATUS::MAX_DMG, itemPower * 0.1f + 0.5f * level));
		break;
	case UNIT_STATUS::MAX_HP:
		mStatus.insert(make_pair(status, itemPower * 2.0f));
		break;
	case UNIT_STATUS::ATK_SPEED:
		mStatus.insert(make_pair(status, itemPower * 0.5f));
		break;
	case UNIT_STATUS::DEF:
	case UNIT_STATUS::HP_REGEN:
	case UNIT_STATUS::VAMP:
	case UNIT_STATUS::COUNTER:
	case UNIT_STATUS::ALL_DMG:
	case UNIT_STATUS::EVASION:
	case UNIT_STATUS::SKEL_QULITY:
	case UNIT_STATUS::SKEL_POWER:
	case UNIT_STATUS::MOVE_SPEED:
		mStatus.insert(make_pair(status, itemPower * 0.1f));
		break;
	}
}

void EquipItem::Init()
{
	type = ITEM_TYPE::EQUIP;
	level = GameData::GetSingleton()->GetLoopLevel();
	lpItemImage = ImageManager::GetSingleton()->FindImage("item");
	lpText = PoolingManager::GetSingleton()->GetClass<Text>();
	lpText->Init("B????????", 14, RGB(255, 255, 255));

	pos = { 0, 0 };
}

void EquipItem::Release()
{
	mStatus.clear();
	lpText->Release();
	lpText = nullptr;
	GameObject::Release();
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

		if (lpText)
		{
			lpText->Render(hdc, (int)pos.x + 3, (int)pos.y + 28);
		}
	}
}

string EquipItem::ToString()
{
	string desc = "";

	int n = (int)UNIT_STATUS::NONE;
	UNIT_STATUS status;
	string var, content = "";
	for (int i = 0; i < n; ++i)
	{
		status = (UNIT_STATUS)i;
		if (mStatus.find(status) != mStatus.end())
		{
			if (content.size() > 0) content += "\n";
			var = to_string(mStatus[status]);
			content += GameData::GetSingleton()->GetLang(status) + ": " + var.substr(0, var.find_first_of('.') + 2);
		}
	}

	return name + "|" + content;
}
