#pragma once
#include "GameObject.h"

enum class UNIT_STATE
{
	INTRO,
	IDLE,
	ATTACK,
	DEATH,
	HURT,
	REVIVE,
	NONE
};

enum class IMAGE_ALIGN;
class Animation;
class Unit;
class BattleUnit : public GameObject
{
private:
	UNIT_STATE state;
	Unit* lpUnit;
	float action;
	bool isAtkReady;

	map<UNIT_STATE, Animation*> mAnimations;

public:
	void Init();
	void Release();
	void Update(float deltaTime);
	void Render(HDC hdc);

	void Intro();
	void Idle();
	void Hit(float dmg);
	void Attack();
	void Revive();
	void Death();

	int GetWidth();
	int GetHeight();

	void SetUnit(Unit* lpUnit);

	inline Unit* GetUnit() { return lpUnit; }
	inline bool IsReady() { return state != UNIT_STATE::INTRO; }
	inline bool IsAlive() { return state != UNIT_STATE::DEATH; }
	inline bool IsAtkReady() { return isAtkReady; }
};

