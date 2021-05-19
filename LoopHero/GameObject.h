#pragma once
#include "LoopHero.h"
#include "EventTrigger.h"

class GameObject : public EventTrigger
{
public:
	virtual ~GameObject() {}

	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(HDC hdc) = 0;
};

