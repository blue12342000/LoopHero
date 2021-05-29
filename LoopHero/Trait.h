#pragma once
#include "GameNode.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

enum class UNIT_STATUS;
enum class EQUIP_PARTS;
enum class UNIT_SLOT;
class Unit;
class EquipItem;
class Trait : public GameNode
{
private:
	string traitId;
	map<UNIT_STATUS, float> mInitStatus;
	vector<UNIT_STATUS> vTraits;
	map<UNIT_SLOT, EQUIP_PARTS> mUnitSlot;
	vector<EQUIP_PARTS> vParts;

private:
	Trait() {}
	Trait(const Trait& other) = delete;

public:
	void Release();

	static Unit* NewUnit(string trait);
	
	EquipItem* CreateEquip();

	inline string GetTraitId() const { return traitId; }
	inline map<UNIT_STATUS, float> GetInitStatus() const { return mInitStatus; }
	inline vector<UNIT_STATUS> GetTraits() const { return vTraits; }
	inline map<UNIT_SLOT, EQUIP_PARTS> GetUnitSlot() const { return mUnitSlot; }
	inline vector<EQUIP_PARTS> GetParts() const { return vParts; }

	friend class GameData;
};

