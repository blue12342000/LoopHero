#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <functional>

using namespace std;

enum class TILE_IMAGE_SEQ
{
	BASIC,
	HORIZON,
	VERTICAL,
	RIGHT_BOTTOM,
	LEFT_BOTTOM,
	RIGHT_TOP,
	LEFT_TOP,
	HORIZON_2,
	VERTICAL_2,
	NONE
};

enum class UNIT_STATUS;
struct EquipInfo
{
	int count;
	vector<string> vName;
	set<UNIT_STATUS> sMainStatus;
	set<UNIT_STATUS> sBonusStatus;
};

enum class TILE_TYPE
{
	TILE,
	SELECT,
	WHITE
};

class Image;
struct Tile
{
	string id;
	string name;
	string desc;
	vector<string> vEtc;
	map<TILE_TYPE, Image*> mLpImage;

	string spawnUnit;
	int spawnPer;
	int spawnDelay;

	// ��ġ�Ҽ� �ִ� ����
	bool checkTiles[3][3];
	int selfCondition;
	int nearCondition;
	vector<string> vSelfTiles;
	vector<string> vNearTiles;

	vector<string> vEventKey;
};

enum class UNIT_SLOT;
enum class EQUIP_PARTS;
class EquipItem;
class Card;
class Deck;
class Unit;
class Hero;
class Trait;
class ObserverHandler;
class FieldTile;
class GameData : public Singleton<GameData>
{
private:
	// Ÿ�Ժ� ����
	map<UNIT_SLOT, string> mUnitSlotLang;
	map<UNIT_STATUS, string> mUnitStatusLang;
	map<EQUIP_PARTS, string> mEquipPartsLang;

	// Ÿ�Ϻ� �����ͼ�
	map<int, TILE_IMAGE_SEQ> mTileSeq;

	// Ű��
	map<string, UNIT_SLOT> mUnitSlot;
	map<string, UNIT_STATUS> mUnitStatus;
	map<string, EQUIP_PARTS> mEquipParts;

	// ���
	map<EQUIP_PARTS, EquipInfo> mEquipInfo;
	// Ư��
	map<string, Trait*> mLpTraits;
	// Ÿ��
	map<string, Tile*> mLpTiles;
	// ��
	Deck* lpDeck;

	map<string, function<void(ObserverHandler*)>> mEventMap;

	int loopLevel;
	Unit* lpUnit;
	Hero* lpHero;

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

	string GetLang(UNIT_SLOT slot);
	string GetLang(UNIT_STATUS status);
	string GetLang(EQUIP_PARTS parts);

	TILE_IMAGE_SEQ GetTileSeq(int data);

	Unit* GetUnit();

	inline void AddEventHandler(string eventKey, function<void(ObserverHandler*)> eventFunc) { mEventMap.insert(make_pair(eventKey, move(eventFunc))); }
	inline function<void(ObserverHandler*)> GetEventHandler(string eventKey) { return mEventMap[eventKey]; }
	inline void ClearEventHandler() { mEventMap.clear(); }
	inline vector<pair<string, Tile*>> GetVMapList() { vector<pair<string, Tile*>> vList(mLpTiles.begin(), mLpTiles.end()); return vList; };
	inline Tile* GetTile(int index) { return (index < mLpTiles.size()) ? next(mLpTiles.begin(), index)->second : nullptr; }
	inline Tile* GetTile(string key) { return (mLpTiles.find(key) != mLpTiles.end()) ? mLpTiles[key] : nullptr; }

	inline int GetLoopLevel() { return loopLevel; }
	inline void SetLoopLevel(int loopLevel) { this->loopLevel = loopLevel; }

	inline void SetHero(Hero* lpHero) { this->lpHero = lpHero; }
	inline Hero* GetHero() { return lpHero; }
	inline void LevelUp() { ++loopLevel; }
};

