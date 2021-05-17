#pragma once
#include <map>
#include <vector>
#include <string>

using namespace std;

enum class UNIT_STATUS;
struct EquipInfo
{
	int count;
	vector<string> vName;
	vector<UNIT_STATUS> vMainStatus;
	vector<UNIT_STATUS> vBonusStatus;
};

enum class EQUIP_PARTS;
class EquipItem;
class EquipTable
{
private:
	map<EQUIP_PARTS, EquipInfo> mEquipInfo;

public:
	const EquipInfo* GetEquipInfo(EQUIP_PARTS parts);

	void LoadEquipData();
};

