#pragma once
#include "GameObject.h"

enum class ITEM_TYPE
{
	EQUIP,
	HP_POTION,
	TROPHIES,
	TROPHIES_POCKET,
	RESOURCE,
	NONE
};

enum class ITEM_RANK
{
	NORMAL,
	RARE,
	UNIQUE,
	REGEND,
	NONE
};

class Image;
class Item : public GameObject
{
protected:
	ITEM_TYPE type;
	ITEM_RANK rank;
	string name;

protected:
	Item() {}

public:
	virtual void Init();
	virtual void Release();
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);
};

