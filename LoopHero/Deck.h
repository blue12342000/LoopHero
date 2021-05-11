#pragma once
#include "GameObject.h"

enum class CARD_KIND
{
	BATTLEFIELD,
	BLOODGROVE,
	CEMETERY,
	CRYSTAL,
	GROVE,
	LANTERN,
	MEADOM,
	MOUNTAIN,
	ROCK,
	OBLIVION,
	SPIDER,
	SWAMP,
	TRESURY,
	VILLAGE,
	NONE
};

const char* CARD_NAME[(int)CARD_KIND::NONE] = {
	"battlefield",
	"bloodgrove",
};

class Image;
struct Card
{
	Image* lpIconImg;
	Image* lpCardImg;

	string name;
	string desc;
};

class Deck : GameObject
{

};