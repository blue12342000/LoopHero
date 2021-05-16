#include "Unit.h"
#include "LoopHero.h"
#include "TraitsTable.h"
#include "EquipItem.h"

void Unit::Init()
{
	mStatus = lpTraits->mInitStatus;
	for (const auto& pair : lpTraits->mUnitEquip)
	{
		mEquip.insert(make_pair(pair.first, EquipSlot{ pair.second, nullptr }));
	}

	mStateImages.insert(make_pair(UNIT_STATE::ICON, ImageManager::GetSingleton()->FindImage(lpTraits->traitId + "_icon")));
	mStateImages.insert(make_pair(UNIT_STATE::IDLE, ImageManager::GetSingleton()->FindImage(lpTraits->traitId + "_idle")));
	mStateImages.insert(make_pair(UNIT_STATE::ATTACK, ImageManager::GetSingleton()->FindImage(lpTraits->traitId + "_attack")));
	mStateImages.insert(make_pair(UNIT_STATE::DEATH, ImageManager::GetSingleton()->FindImage(lpTraits->traitId + "_death")));
	mStateImages.insert(make_pair(UNIT_STATE::HURT, ImageManager::GetSingleton()->FindImage(lpTraits->traitId + "_hurt")));
	mStateImages.insert(make_pair(UNIT_STATE::REVIVE, ImageManager::GetSingleton()->FindImage(lpTraits->traitId + "_revive")));
}

void Unit::Release()
{
}

void Unit::Update(float deltaTime)
{
}

void Unit::Render(HDC hdc)
{
}
