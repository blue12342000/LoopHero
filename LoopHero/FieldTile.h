#pragma once
#include "GameObject.h"

enum class TILE_DIRECTION
{
	LEFT,
	UP,
	RIGHT,
	DOWN,
	NONE
};

enum class TILE_TYPE;
class Tile;
class FieldTileMap;
class FieldTile : public GameObject
{
private:
	TILE_TYPE type;
	TILE_DIRECTION direction;

	int x;
	int y;

	Tile* lpTile;
	int frameX;
	int frameY;

	float angle;
	float radius;

	vector<string> vHistory;
	FieldTile* lpNearTiles[(int)TILE_DIRECTION::NONE];

	int spawnDelay;

public:
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	void DailySpawnMonster(ObserverHandler* lpCaller);
	void DailyNearSpawnMonster(ObserverHandler* lpCaller);
	void ClearMonster();

	void OnMouseOver(EventData& data) override;
	void OnMouseOut(EventData& data) override;

	string ToString() override;

	inline POINT GetTilePos() { return POINT{x, y}; }
	inline int GetTilePosX() { return x; }
	inline int GetTilePosY() { return y; }
	inline Tile* GetTile() { return lpTile; }
	inline void AddHistory(string tileKey) { vHistory.push_back(tileKey); }

	friend class FieldTileMap;
};

