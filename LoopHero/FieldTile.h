#pragma once
#include "GameObject.h"

class Tile;
class FieldTile2 : public GameObject
{
private:
	int x;
	int y;

	Tile* lpTile;
	int frameX;
	int frameY;
	int eventCount;

	vector<string> vHistory;

public:
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	inline void SetXY(int x, int y) { this->x = x; this->y = y; }
	inline void SetTile(Tile* lpTile) { this->lpTile = lpTile; }
	inline void SetFrame(int x, int y) { frameX = x; frameY = y; }
	inline void AddHistory(string tileKey) { vHistory.push_back(tileKey); }
};

