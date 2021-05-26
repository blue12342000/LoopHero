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

class EquipItem;
enum class EQUIP_PARTS;
struct EquipSlot
{
	EQUIP_PARTS parts;
	EquipItem* lpEquip;
};

class Trait;
class Animation;
class Unit : public GameObject
{
private:
	string name;
	float currHp;

	Animation* lpIcon;

	Trait* lpTrait;
	map<UNIT_STATUS, float> mStatus;
	map<UNIT_SLOT, EquipSlot> mEquip;

	vector<EquipItem*> vEquipInven;

public:
	virtual void Init();
	virtual void Release();
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);

	void Idle();
	void Hit(float dmg);
	void Attack();
	void Revive();
	void Death();

	void SetTrait(Trait& trait);

	string ToString() override;

	inline float GetCurrHp() { return currHp; }
	inline Trait* GetTrait() { return lpTrait; }
	inline float GetStatus(UNIT_STATUS status) { if (mStatus.find(status) == mStatus.end()) { return 0.0f; } else { return mStatus[status]; } }
};

