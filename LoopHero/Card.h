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

class Card : public GameObject
{
private:
	CARD_TYPE type;
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

	void LoadCardInfo(CARD_TYPE type, string card, string tileKey);

	inline Image* GetIconImg() { return lpIconImg; }
	inline void SetPos(POINTFLOAT pos) { this->pos = pos; }
};

