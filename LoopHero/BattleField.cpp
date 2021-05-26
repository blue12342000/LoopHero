#include "BattleField.h"
#include "BattleUnit.h"
#include "Unit.h"

void BattleField::Init()
{
}

void BattleField::Release()
{
	lDeath.clear();
	lHeroParty.clear();
	lEnemyParty.clear();
}

void BattleField::Update(float deltaTime)
{
	for (BattleUnit*& lpBattleUnit : lHeroParty)
	{
		lpBattleUnit->Update(deltaTime);
		if (lpBattleUnit->IsAtkReady())
		{
			Attack(lpBattleUnit, lEnemyParty);
		}
	}

	for (BattleUnit*& lpBattleUnit : lEnemyParty)
	{
		lpBattleUnit->Update(deltaTime);
		if (lpBattleUnit->IsAtkReady())
		{
			Attack(lpBattleUnit, lHeroParty);
		}
	}

	//GameObject::Update(deltaTime);
}

void BattleField::Render(HDC hdc)
{
	//GameObject::Render(hdc);
}

void BattleField::Attack(BattleUnit*& lpAttaker, list<BattleUnit*>& lDefenders)
{
	lpAttaker->Attack();
	for (auto& member : lDefenders)
	{
		member->Hit(0.0f);
	}
}

void BattleField::AddUnit(BATTLE_TEAM team, Unit* lpUnit)
{
	if (lpUnit)
	{
		BattleUnit* lpBattleUnit = GameObject::Create<BattleUnit>();
		lpBattleUnit->SetUnit(lpUnit);
		switch (team)
		{
		case BATTLE_TEAM::LEFT:
			lHeroParty.push_back(lpBattleUnit);
			break;
		case BATTLE_TEAM::RIGHT:
			lEnemyParty.push_back(lpBattleUnit);
			break;
		}
		lpBattleUnit->Intro();
	}
}