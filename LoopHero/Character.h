#pragma once
#include "GameObject.h"

enum class CARACTER_STATE
{
	IDLE,
	ATTACK,
	DEATH,
	HURT,
	REVIVE,
	NONE
};

class Animation;
class Character
{
private:
	CARACTER_STATE state;
	map<CARACTER_STATE, Animation*> mAnimations;

public:
	void Init(string id);
	void Release();
	void Update(float deltaTime);
	void Render(HDC hdc, float x, float y);

	void Idle();
	void Hit();
	void Attack();
	void Revive();
	void Death();
};

