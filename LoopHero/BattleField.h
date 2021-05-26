#pragma once
#include "GameObject.h"

enum class BATTLE_TEAM
{
	LEFT,
	RIGHT
};

class Unit;
class BattleUnit;
class BattleField : public GameObject
{
private:
	list<BattleUnit*> lDeath;
	list<BattleUnit*> lHeroParty;
	list<BattleUnit*> lEnemyParty;

public:
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	void Attack(BattleUnit*& lpAttaker, list<BattleUnit*>& lDefenders);
	void AddUnit(BATTLE_TEAM team, Unit* lpUnit);

	inline list<BattleUnit*> GetHeroParty() { return lHeroParty; }
	inline list<BattleUnit*> GetEnemyParty() { return lEnemyParty; }
	inline bool IsFinish() { return lHeroParty.empty() || lEnemyParty.empty(); }
};

