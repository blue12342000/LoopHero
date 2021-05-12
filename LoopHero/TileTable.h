#pragma once
#include "GameObject.h"
#include "Utill.h"

class Image;
struct Tile
{
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

class TileTable
{
private:
	map<string, Tile*> mLpTiles;

	vector<RECT> vRects;

public:
	void LoadTileData();
	void Release();
	void Update();
	void Render(HDC hdc);

	inline vector<RECT> GetVRects() { return vRects; }
	inline vector<pair<string, Tile*>> GetVMapList() { vector<pair<string, Tile*>> vList(mLpTiles.begin(), mLpTiles.end()); return vList; };
	inline Tile* GetTile(int index) { return (index < mLpTiles.size()) ? next(mLpTiles.begin(), index)->second : nullptr; }
	inline Tile* GetTile(string key) { return (mLpTiles.find(key) != mLpTiles.end()) ? mLpTiles[key] : nullptr; }
};
