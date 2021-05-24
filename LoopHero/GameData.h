#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <vector>
#include <string>
#include <set>
#include <map>

using namespace std;

enum class UNIT_STATUS;
struct EquipInfo
{
	int count;
	vector<string> vName;
	set<UNIT_STATUS> sMainStatus;
	set<UNIT_STATUS> sBonusStatus;
};

class Image;
struct Tile
{
	string id;
	string name;
	string desc;
	vector<string> vEtc;

	// 설치할수 있는 정보
	bool checkTiles[3][3];
	int selfCondition;
	int nearCondition;
	vector<string> vSelfTiles;
	vector<string> vNearTiles;
};

enum class UNIT_SLOT;
enum class EQUIP_PARTS;
class EquipItem;

class Card;
class Deck;
class TraitsTable;
class EquipTable;
class Unit;
class Trait;
class Tile;
class GameData : public Singleton<GameData>
{
private:
	map<string, UNIT_SLOT> mUnitSlot;
	map<string, UNIT_STATUS> mUnitStatus;
	map<string, EQUIP_PARTS> mEquipParts;

	// 장비
	map<EQUIP_PARTS, EquipInfo> mEquipInfo;
	// 특성
	map<string, Trait*> mLpTraits;
	// 타일
	map<string, Tile*> mLpTiles;
	// 덱
	Deck* lpDeck;

	int loopLevel;
	Unit* lpUnit;

private:
	void LoadTraits();
	void LoadTiles();
	void LoadEquipInfo();

public:
	HRESULT Init();
	void Release();

	void Save() {}
	void Load() {}

	Card* PickCard();

	const EquipInfo* GetEquipInfo(EQUIP_PARTS parts);
	Trait* GetTrait(string traitKey);

	inline vector<pair<string, Tile*>> GetVMapList() { vector<pair<string, Tile*>> vList(mLpTiles.begin(), mLpTiles.end()); return vList; };
	inline Tile* GetTile(int index) { return (index < mLpTiles.size()) ? next(mLpTiles.begin(), index)->second : nullptr; }
	inline Tile* GetTile(string key) { return (mLpTiles.find(key) != mLpTiles.end()) ? mLpTiles[key] : nullptr; }

	inline int GetLoopLevel() { return loopLevel; }
	inline void SetUnit(Unit* lpUnit) { this->lpUnit = lpUnit; }
	inline Unit* GetUnit() { return lpUnit; }
};

