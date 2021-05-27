#pragma once
#include "Scene.h"

class Image;
class FieldTileMap;
class GameUI;
class EventSystem;
class UIBattleWindow;
class InGame : public Scene
{
private:
	Image* lpBuffer;
	Image* lpBackImage;

	GameUI* lpCanvus;

	FieldTileMap* lpFieldTiles;
	UIBattleWindow* lpBattleWindow;

	EventSystem* lpEventSystem;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
};

