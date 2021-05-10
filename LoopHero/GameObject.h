#pragma once
#include "LoopHero.h"

class GameObject
{
public:
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(HDC hdc) = 0;
};

