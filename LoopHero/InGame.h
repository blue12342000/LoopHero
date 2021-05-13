#pragma once
#include "Scene.h"

class Image;
class FieldTileMap;
class TileTable;
class Deck;
class GameUI;
class InGame : public Scene
{
private:
	Image* lpBuffer;
	Image* lpBackImage;

	GameUI* lpRightMenu;

	FieldTileMap* lpFieldTiles;
	Deck* lpDeck;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
};

