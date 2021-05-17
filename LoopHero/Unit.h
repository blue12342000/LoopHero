#pragma once
#include "GameObject.h"

enum class UNIT_STATUS
{
	DMG,
	MIN_DMG,
	MAX_DMG,
	DEF,
	MAX_HP,
	HP_REGEN,
	VAMP,
	COUNTER,
	ALL_DMG,
	EVASION,
	SKEL_QULITY,
	SKEL_POWER,
	MOVE_SPEED,
	ATK_SPEED,
	NONE
};

enum class UNIT_SLOT
{
	LEFT_HAND,
	RIGHT_HAND,
	HELMET,
	ARMOR,
	BELTS,
	GLOVES,
	BOOTS,
	LEFT_RING,
	RIGHT_RING,
	ARMULET,
	NONE
};

enum class UNIT_STATE
{
	ICON,
	IDLE,
	ATTACK,
	DEATH,
	HURT,
	REVIVE
};

class EquipItem;
enum class EQUIP_PARTS;
struct EquipSlot
{
	EQUIP_PARTS parts;
	EquipItem* lpEquip;
};

class Traits;
class Unit : public GameObject
{
private:
	map<UNIT_STATE, Image*> mStateImages;
	map<UNIT_STATUS, float> mStatus;
	map<UNIT_SLOT, EquipSlot> mEquip;
	Traits* lpTraits;

	float currHp;

private:
	Unit() {}

public:
	virtual void Init();
	virtual void Release();
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);

	inline float GetCurrHp() { return currHp; }
	inline const Traits* GetTraits() { return lpTraits; }

	friend class TraitsTable;
};

