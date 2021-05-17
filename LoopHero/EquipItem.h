#pragma once
#include "Item.h"

enum class EQUIP_PARTS
{
	SWORD,
	BOOK,
	SHIELD,
	HELMET,
	ARMOR,
	GLOVES,
	BELTS,
	BOOTS,
	RING,
	ARMULET,
	NONE
};

enum class UNIT_STATUS;
class Traits;
class EquipItem : public Item
{
private:
	int level;
	EQUIP_PARTS parts;
	map<UNIT_STATUS, float> mStatus;

private:
	EquipItem() {}

public:
	virtual void Init();
	virtual void Release();
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);

	static EquipItem* CreateEquip(Traits* lpTraits);

};

