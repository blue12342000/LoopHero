#pragma once
#include "GameObject.h"

#define FIELD_START_X 0
#define FIELD_START_Y 54

#define FIELD_TILE_X 21
#define FIELD_TILE_Y 12

#define FIELD_TILE_SIZE 50

class Tile;
struct FieldTile
{
	int x;
	int y;
	RECT rc;

	Tile* lpTile;
	int frameX;
	int frameY;
	int eventCount;

	vector<string> vHistory;
};

class TileTable;
class FieldTileMap : public GameObject
{
private:
	FieldTile tiles[FIELD_TILE_Y][FIELD_TILE_X];

	map<string, vector<FieldTile*>> mBuildTiles;

	Tile* lpSelectedTile;
	bool isPossibleBuild[FIELD_TILE_Y][FIELD_TILE_X];
	TileTable* lpTileTable;

public:
	virtual ~FieldTileMap() {}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	bool BuildTile(int x, int y, Tile* lpTile);
	void SelectedTileValidation();
	void SelectedCard(ObserverHandler* lpObserver);
	void DeselectCard(ObserverHandler* lpObserver);

	inline void SetTile(int x, int y, Tile* lpTile) { tiles[y][x].lpTile = lpTile; }

	virtual void OnClick(EventData& data) override;
	virtual void OnMouseEnter(EventData& data) override;
	virtual void OnMouseOver(EventData& data) override;
	virtual void OnMouseOut(EventData& data) override;
	virtual void OnDrop(EventData& data) override;
};