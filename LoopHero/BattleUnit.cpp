#include "BattleUnit.h"
#include "Animation.h"
#include "Image.h"
#include "Unit.h"
#include "Trait.h"

void BattleUnit::Init()
{
	state = UNIT_STATE::INTRO;
	lpUnit = nullptr;
	action = 0;
	maxAction = 1.0f;
}

void BattleUnit::Release()
{
	for (auto pair : mAnimations)
	{
		delete pair.second;
	}
	mAnimations.clear();

	lpUnit = nullptr;
}

void BattleUnit::Update(float deltaTime)
{
	action += (1.0f + lpUnit->GetStatus(UNIT_STATUS::ATK_SPEED) * 0.01f) * deltaTime;
	if (action >= maxAction)
	{
		isAtkReady = true;
		action = maxAction;
	}

	mAnimations[state]->Update(deltaTime);
}

void BattleUnit::Render(HDC hdc)
{
	mAnimations[state]->Render(hdc, pos.x, pos.y, IMAGE_ALIGN::LEFT_TOP);
}

void BattleUnit::Intro()
{
	mAnimations[state]->Stop();
	state = UNIT_STATE::INTRO;
	mAnimations[state]->Play();
}

void BattleUnit::Idle()
{
	mAnimations[state]->Stop();
	state = UNIT_STATE::IDLE;
	mAnimations[state]->Play();
}

void BattleUnit::Hit(float dmg)
{
	mAnimations[state]->Stop();
	state = UNIT_STATE::HURT;
	mAnimations[state]->Play();

	if (lpUnit) lpUnit->Hit(dmg);
}

void BattleUnit::Attack()
{
	mAnimations[state]->Stop();
	state = UNIT_STATE::ATTACK;
	mAnimations[state]->Play();

	action = 0;
	isAtkReady = false;
}

void BattleUnit::Revive()
{
	mAnimations[state]->Stop();
	state = UNIT_STATE::REVIVE;
	mAnimations[state]->Play();
}

void BattleUnit::Death()
{
	mAnimations[state]->Stop();
	state = UNIT_STATE::DEATH;
	mAnimations[state]->Play();
}

int BattleUnit::GetWidth()
{
	if (mAnimations.find(state) == mAnimations.end()) { return 0.0f; }
	else { return mAnimations[state]->GetWidth(); }
}

int BattleUnit::GetHeight()
{
	if (mAnimations.find(state) == mAnimations.end()) { return 0.0f; }
	else { return mAnimations[state]->GetHeight(); }
}

void BattleUnit::SetUnit(Unit* lpUnit)
{
	Init();

	this->lpUnit = lpUnit;
	this->action = 0;

	string id = lpUnit->GetTrait()->GetTraitId();
	mAnimations.insert(make_pair(UNIT_STATE::INTRO, new Animation()));
	mAnimations[UNIT_STATE::INTRO]->Init(ImageManager::GetSingleton()->FindImage(id + "_intro"), ANIMATION_TYPE::LOOP, 10);
	mAnimations[UNIT_STATE::INTRO]->SetCallBack(bind(&BattleUnit::Idle, this));

	mAnimations.insert(make_pair(UNIT_STATE::IDLE, new Animation()));
	mAnimations[UNIT_STATE::IDLE]->Init(ImageManager::GetSingleton()->FindImage(id + "_idle"), ANIMATION_TYPE::LOOP, 10);

	mAnimations.insert(make_pair(UNIT_STATE::ATTACK, new Animation()));
	mAnimations[UNIT_STATE::ATTACK]->Init(ImageManager::GetSingleton()->FindImage(id + "_attack"), ANIMATION_TYPE::ONCE, 10);
	mAnimations[UNIT_STATE::ATTACK]->SetCallBack(bind(&BattleUnit::Idle, this));

	mAnimations.insert(make_pair(UNIT_STATE::DEATH, new Animation()));
	mAnimations[UNIT_STATE::DEATH]->Init(ImageManager::GetSingleton()->FindImage(id + "_death"), ANIMATION_TYPE::ONCE, 10);
	//mAnimations[UNIT_STATE::DEATH]->SetCallBack(bind(&Character::Death, this));

	mAnimations.insert(make_pair(UNIT_STATE::HURT, new Animation()));
	mAnimations[UNIT_STATE::HURT]->Init(ImageManager::GetSingleton()->FindImage(id + "_hurt"), ANIMATION_TYPE::ONCE, 10);
	mAnimations[UNIT_STATE::HURT]->SetCallBack(bind(&BattleUnit::Idle, this));

	mAnimations.insert(make_pair(UNIT_STATE::REVIVE, new Animation()));
	mAnimations[UNIT_STATE::REVIVE]->Init(ImageManager::GetSingleton()->FindImage(id + "_revive"), ANIMATION_TYPE::ONCE, 10);
	mAnimations[UNIT_STATE::REVIVE]->SetCallBack(bind(&BattleUnit::Idle, this));
}
