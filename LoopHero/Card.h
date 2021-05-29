#pragma once
#include "GameObject.h"

enum class CARD_TYPE
{
	ROAD,
	ROAD_SIDE,
	ENVIRONMENT,
	SPECIAL,
	NONE
};

enum class CARD_RARE
{
	NORMAL,
	RARE,
	UNIQUE,
	NONE
};

class Deck;
class Card : public GameObject
{
private:
	CARD_TYPE type;
	CARD_RARE rare;
	Image* lpIconImg;
	Image* lpCardImg;

	POINTFLOAT pos;
	string id;
	string name;
	string desc;

	string tileKey;

	RECT rc;
	int width;
	int height;

	HFONT hFont;
	HFONT hOldFont;

public:
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	inline string GetId() { return id; }
	inline Image* GetIconImg() { return lpIconImg; }
	inline Image* GetCardImg() { return lpCardImg; }
	inline string GetTileKey() { return tileKey; }

	string ToString() override;

	friend class Deck;
};


