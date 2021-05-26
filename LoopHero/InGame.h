#pragma once
#include "Scene.h"

class Image;
class FieldTileMap;
class GameUI;
class EquipItem;
class Unit;
class EventSystem;
class UIBattleWindow;
class BattleField;
class InGame : public Scene
{
private:
	Image* lpBuffer;
	Image* lpBackImage;

	GameUI* lpCanvus;

	FieldTileMap* lpFieldTiles;

	Unit* lpUnit;
	Unit* lpEnemy;
	EquipItem* lpEquipItem;

	BattleField* lpBattleField;
	UIBattleWindow* lpBattleWindow;

	EventSystem* lpEventSystem;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
};

