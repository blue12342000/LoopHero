#include "BattleUnit.h"
#include "Animation.h"
#include "Image.h"
#include "Unit.h"
#include "Trait.h"
#include "ParticleManager.h"

void BattleUnit::Init()
{
	state = UNIT_STATE::INTRO;
	lpUnit = nullptr;
	action = 0;
	maxAction = 1.0f;
	isAtkReady = false;
}

void BattleUnit::Release()
{
	lpUnit = nullptr;
	for (const auto& pair : mAnimations)
	{
		pair.second->Release();
	}
	mAnimations.clear();
	GameObject::Release();
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
	// 해당 오르젝트는 UI에서 그려질것이라 예외적으로 항상 0.0f에서 그려지도록 한다.
	mAnimations[state]->Render(hdc, 0.0f, 0.0f, IMAGE_ALIGN::LEFT_TOP);
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
	if (!lpUnit->IsAlive()) return;

	if (lpUnit)
	{
		float def = lpUnit->GetStatus(UNIT_STATUS::DEF);
		dmg -= def;
		if (dmg < 0) dmg = 0.1f;

		if (lpUnit->Hit(dmg))
		{
			string dmgStr = to_string(dmg);
			dmgStr = "#0000AA|" + dmgStr.substr(0, dmgStr.find('.') + 2);
			ParticleManager::GetSingleton()->SpreadParticle("Battle_ParticleSystem", POINTFLOAT{pos.x, pos.y - mAnimations[state]->GetHeight() / 2 }, dmgStr);
			// 맞음
			if (!lpUnit->IsAlive())
			{
				// 죽음
				Death();
			}
			else
			{
				mAnimations[state]->Stop();
				state = UNIT_STATE::HURT;
				mAnimations[state]->Play();
			}
		}
		else
		{
			// 회피함
			ParticleManager::GetSingleton()->SpreadParticle("Battle_ParticleSystem", GetWorldPos(), "회피");
		}
	}
}

float BattleUnit::Attack()
{
	mAnimations[state]->Stop();
	state = UNIT_STATE::ATTACK;
	mAnimations[state]->Play();

	action = 0;
	isAtkReady = false;
	return lpUnit->Attack();
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
	this->lpUnit = lpUnit;
	this->action = 0;

	string id = lpUnit->GetTrait()->GetTraitId();
	mAnimations.insert(make_pair(UNIT_STATE::INTRO, PoolingManager::GetSingleton()->GetClass<Animation>()));
	mAnimations[UNIT_STATE::INTRO]->Init(ImageManager::GetSingleton()->FindImage(id + "_intro"), ANIMATION_TYPE::LOOP, 10);
	mAnimations[UNIT_STATE::INTRO]->SetCallBack(bind(&BattleUnit::Idle, this));

	mAnimations.insert(make_pair(UNIT_STATE::IDLE, PoolingManager::GetSingleton()->GetClass<Animation>()));
	mAnimations[UNIT_STATE::IDLE]->Init(ImageManager::GetSingleton()->FindImage(id + "_idle"), ANIMATION_TYPE::LOOP, 10);

	mAnimations.insert(make_pair(UNIT_STATE::ATTACK, PoolingManager::GetSingleton()->GetClass<Animation>()));
	mAnimations[UNIT_STATE::ATTACK]->Init(ImageManager::GetSingleton()->FindImage(id + "_attack"), ANIMATION_TYPE::ONCE, 10);
	mAnimations[UNIT_STATE::ATTACK]->SetCallBack(bind(&BattleUnit::Idle, this));

	mAnimations.insert(make_pair(UNIT_STATE::DEATH, PoolingManager::GetSingleton()->GetClass<Animation>()));
	mAnimations[UNIT_STATE::DEATH]->Init(ImageManager::GetSingleton()->FindImage(id + "_death"), ANIMATION_TYPE::ONCE, 10);
	//mAnimations[UNIT_STATE::DEATH]->SetCallBack(bind(&Character::Death, this));

	mAnimations.insert(make_pair(UNIT_STATE::HURT, PoolingManager::GetSingleton()->GetClass<Animation>()));
	mAnimations[UNIT_STATE::HURT]->Init(ImageManager::GetSingleton()->FindImage(id + "_hurt"), ANIMATION_TYPE::ONCE, 10);
	mAnimations[UNIT_STATE::HURT]->SetCallBack(bind(&BattleUnit::Idle, this));

	mAnimations.insert(make_pair(UNIT_STATE::REVIVE, PoolingManager::GetSingleton()->GetClass<Animation>()));
	mAnimations[UNIT_STATE::REVIVE]->Init(ImageManager::GetSingleton()->FindImage(id + "_revive"), ANIMATION_TYPE::ONCE, 10);
	mAnimations[UNIT_STATE::REVIVE]->SetCallBack(bind(&BattleUnit::Idle, this));
}
