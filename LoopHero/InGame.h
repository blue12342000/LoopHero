#pragma once
#include "Scene.h"

class Image;
class FieldTileMap;
class TileTable;
class InGame : public Scene
{
private:
	Image* lpBuffer;
	Image* lpBackImage;

	FieldTileMap* lpFieldTiles;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
};

