#pragma once
#include <map>
#include <string>
#include <vector>

using namespace std;

enum class UNIT_STATUS;
enum class EQUIP_PARTS;
enum class UNIT_SLOT;
struct Traits
{
	string traitId;
	map<UNIT_STATUS, float> mInitStatus;
	vector<UNIT_STATUS> vTraits;
	map<UNIT_SLOT, EQUIP_PARTS> mUnitEquip;
	vector<EQUIP_PARTS> vEquips;
};

class Unit;
class TraitsTable
{
private:
	map<string, Traits*> mLpTraits;

public:
	void LoadTraitsData();
	Unit* CreateUnit(string traitKey);
};

