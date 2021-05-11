#pragma once
#include "GameObject.h"

enum class TILE_IMAGE_TYPE
{
	SELECT,
	BUILD
};

class Image;
struct Tile
{
	int no;
	string name;
	string desc;
	string effect;
	map<TILE_IMAGE_TYPE, Image*> mTileImages;
};

class TileTable
{
private:
	vector<Tile*> vLpTiles;

public:
	void LoadTileList();

};
