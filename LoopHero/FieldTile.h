#pragma once
#include "GameObject.h"

class Tile;
class FieldTileMap;
class FieldTile : public GameObject
{
private:
	int x;
	int y;

	Tile* lpTile;
	int frameX;
	int frameY;
	int eventCount;

	float angle;
	float radius;

	vector<string> vHistory;

public:
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	void DailySpawnMonster(ObserverHandler* lpCaller);

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

