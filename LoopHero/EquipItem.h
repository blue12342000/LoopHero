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
class Trait;
class Text;
class EquipItem : public Item
{
private:
	int no;
	int level;
	EQUIP_PARTS parts;
	map<UNIT_STATUS, float> mStatus;
	Text* lpText;

private:
	void SetItemStatus(UNIT_STATUS status, int itemPower);
	
public:
	virtual void Init();
	virtual void Release();
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);

	inline EQUIP_PARTS GetParts() { return parts; }
	inline map<UNIT_STATUS, float> GetStatus() { return mStatus; }

	string ToString() override;

	friend class Trait;
};

