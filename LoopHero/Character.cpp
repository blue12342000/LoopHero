#include "Character.h"
#include "Animation.h"

void Character::Init(string id)
{
	state = CARACTER_STATE::IDLE;

	mAnimations.insert(make_pair(CARACTER_STATE::IDLE, new Animation()));
	mAnimations[CARACTER_STATE::IDLE]->Init(ImageManager::GetSingleton()->FindImage(id + "_idle"), ANIMATION_TYPE::LOOP, 5);

	mAnimations.insert(make_pair(CARACTER_STATE::ATTACK, new Animation()));
	mAnimations[CARACTER_STATE::ATTACK]->Init(ImageManager::GetSingleton()->FindImage(id + "_attack"), ANIMATION_TYPE::ONCE, 5);
	mAnimations[CARACTER_STATE::ATTACK]->SetCallBack(bind(&Character::Idle, this));

	mAnimations.insert(make_pair(CARACTER_STATE::DEATH, new Animation()));
	mAnimations[CARACTER_STATE::DEATH]->Init(ImageManager::GetSingleton()->FindImage(id + "_death"), ANIMATION_TYPE::ONCE, 5);
	//mAnimations[CARACTER_STATE::ATTACK]->SetCallBack(bind(&Character::Death, this));

	mAnimations.insert(make_pair(CARACTER_STATE::HURT, new Animation()));
	mAnimations[CARACTER_STATE::HURT]->Init(ImageManager::GetSingleton()->FindImage(id + "_hurt"), ANIMATION_TYPE::ONCE, 5);
	mAnimations[CARACTER_STATE::ATTACK]->SetCallBack(bind(&Character::Idle, this));

	mAnimations.insert(make_pair(CARACTER_STATE::REVIVE, new Animation()));
	mAnimations[CARACTER_STATE::REVIVE]->Init(ImageManager::GetSingleton()->FindImage(id + "_revive"), ANIMATION_TYPE::ONCE, 5);
	mAnimations[CARACTER_STATE::ATTACK]->SetCallBack(bind(&Character::Idle, this));
}

void Character::Release()
{
	for (auto pair : mAnimations)
	{
		delete pair.second;
	}
	mAnimations.clear();
}

void Character::Update(float deltaTime)
{
	mAnimations[state]->Update(deltaTime);
}

void Character::Render(HDC hdc, float x, float y)
{
	mAnimations[state]->Render(hdc, (int)x, (int)y);
}

void Character::Idle()
{
	mAnimations[state]->Stop();
	state = CARACTER_STATE::IDLE;
	mAnimations[state]->Play();
}

void Character::Hit()
{
	mAnimations[state]->Stop();
	state = CARACTER_STATE::HURT;
	mAnimations[state]->Play();
}

void Character::Attack()
{
	mAnimations[state]->Stop();
	state = CARACTER_STATE::ATTACK;
	mAnimations[state]->Play();
}

void Character::Revive()
{
	mAnimations[state]->Stop();
	state = CARACTER_STATE::REVIVE;
	mAnimations[state]->Play();
}

void Character::Death()
{
	mAnimations[state]->Stop();
	state = CARACTER_STATE::DEATH;
	mAnimations[state]->Play();
}
