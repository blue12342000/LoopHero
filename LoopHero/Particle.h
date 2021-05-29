#pragma once
#include "GameObject.h"

enum class PATTICLE_TYPE
{
	IMAGE,
	TEXT
};

class Text;
class Animation;
class Particle : public GameObject
{
private:
	PATTICLE_TYPE type;
	Animation* lpAnimation;
	Text* lpText;

public:
	void Init();
	void Release();
	void Render(HDC hdc);

};

