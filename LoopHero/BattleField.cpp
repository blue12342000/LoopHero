#include "BattleField.h"
#include "BattleUnit.h"
#include "Unit.h"

void BattleField::Init()
{
}

void BattleField::Release()
{
	for (auto& lpBattleUnit : lDeath)
	{
		lpBattleUnit->Release();
	}

	for (auto& lpBattleUnit : lHeroParty)
	{
		lpBattleUnit->Release();
	}

	for (auto& lpBattleUnit : lEnemyParty)
	{
		lpBattleUnit->Release();
	}

	lDeath.clear();
	lHeroParty.clear();
	lEnemyParty.clear();

	PoolingManager::GetSingleton()->AddClass(this);
}

void BattleField::Update(float deltaTime)
{
	BattleUnit* lpBattleUnit;
	for (auto it = lDeath.begin(); it != lDeath.end(); ++it)
	{
		(*it)->Update(deltaTime);
	}

	for (auto it = lHeroParty.begin(); it != lHeroParty.end(); ++it)
	{
		lpBattleUnit = (*it);
		lpBattleUnit->Update(deltaTime);
		if (lpBattleUnit->IsAtkReady())
		{
			if (Attack(lpBattleUnit, lEnemyParty))
			{
				ObserverManager::GetSingleton()->Notify("DropCard", this);
				ObserverManager::GetSingleton()->Notify("DropEquip", lHeroParty.front()->GetUnit());
			}
		}
	}

	for (auto it = lEnemyParty.begin(); it != lEnemyParty.end(); ++it)
	{
		lpBattleUnit = (*it);
		lpBattleUnit->Update(deltaTime);
		if (lpBattleUnit->IsAtkReady())
		{
			Attack(lpBattleUnit, lHeroParty);
		}
	}

	if (IsFinish())
	{
		ObserverManager::GetSingleton()->Notify("BattleEnd", this);
		return;
	}

	//GameObject::Update(deltaTime);
}

void BattleField::Render(HDC hdc)
{
	//GameObject::Render(hdc);
}

bool BattleField::Attack(BattleUnit*& lpAttaker, list<BattleUnit*>& lDefenders)
{
	if (lDefenders.empty()) return false;

	;
	int randNum = rand() % lDefenders.size();
	auto it = lDefenders.begin();
	advance(it, randNum);
	(*it)->Hit(lpAttaker->Attack());
	
	if ((*it)->IsDeath())
	{
		lDeath.push_front(*it);
		lDefenders.erase(it);
		return true;
	}

	return false;
}

void BattleField::AllAttack(BattleUnit*& lpAttaker, list<BattleUnit*>& lDefenders)
{
	if (lDefenders.empty()) return;

	lpAttaker->Attack();
	for (auto& member : lDefenders)
	{
		member->Hit(0.3f);
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