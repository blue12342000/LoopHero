#pragma once
#include "GameObject.h"

enum class TILE_TYPE
{

};

class Image;
class Tile : public GameObject
{
private:
	map<TILE_TYPE, Image*> mTileImages;

public:
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
};

