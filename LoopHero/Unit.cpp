#include "Unit.h"
#include "LoopHero.h"
#include "Trait.h"
#include "EquipItem.h"
#include "Image.h"
#include "Animation.h"
#include "Character.h"

void Unit::Init()
{
	lpIcon = nullptr;
	lpCharacter = nullptr;
}

void Unit::Release()
{
	lpTrait = nullptr;

	if (lpIcon)
	{
		delete lpIcon;
		lpIcon = nullptr;
	}

	if (lpCharacter)
	{
		lpCharacter->Release();
		delete lpCharacter;
		lpCharacter = nullptr;
	}
}

void Unit::Update(float deltaTime)
{
	if (KeyManager::GetSingleton()->IsKeyOnceDown('H'))
	{
		lpCharacter->Attack();
		lpIcon->Play();
	}

	if (lpIcon) lpIcon->Update(deltaTime);
	if (lpCharacter) lpCharacter->Update(deltaTime);
}

void Unit::Render(HDC hdc)
{
	if (lpIcon) lpIcon->Render(hdc, pos.x, pos.y);
	if (lpCharacter) lpCharacter->Render(hdc, pos.x + 100, pos.y);
}

void Unit::Idle()
{
}

void Unit::Hit(float dmg)
{
}

void Unit::Attack()
{
}

void Unit::Revive()
{
}

void Unit::Death()
{
}

void Unit::SetTrait(Trait& trait)
{
	Release();
	this->lpTrait = &trait;

	name = lpTrait->GetTraitId();
	mStatus = lpTrait->GetInitStatus();
	for (const auto& pair : lpTrait->GetUnitSlot())
	{
		mEquip.insert(make_pair(pair.first, EquipSlot{ pair.second, nullptr }));
	}
	lpIcon = new Animation();
	lpIcon->Init(name + "_icon", ANIMATION_TYPE::LOOP, 5);

	lpCharacter = new Character();
	lpCharacter->Init(name);
}
