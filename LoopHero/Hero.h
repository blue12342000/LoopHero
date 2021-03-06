#pragma once
#include "GameObject.h"

enum class HERO_STATE
{
	IDLE,
	MOVE,
	BATTLE
};

class Unit;
class FieldTile;
class Hero : public GameObject
{
private:
	struct LoopIterator
	{
		vector<FieldTile*>::iterator iter;
		vector<FieldTile*>::iterator beginIter;
		vector<FieldTile*>::iterator endIter;

		LoopIterator() {}
		LoopIterator(vector<FieldTile*>& vFieldTile)
		{
			this->iter = vFieldTile.begin();
			this->beginIter = vFieldTile.begin();
			this->endIter = vFieldTile.end();
		}

		LoopIterator& operator++()
		{
			++iter;
			if (iter == endIter) iter = beginIter;
			return *this;
		}
	};

private:
	HERO_STATE state;
	Unit* lpUnit;
	LoopIterator tileIter;

public:
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	void Move(ObserverHandler* lpCaller);

	void Loop(vector<FieldTile*>& vMovePath);
	void NewHero(string id);

	inline Unit* GetUnit() { return lpUnit; }
	inline bool IsLoop() { return state != HERO_STATE::IDLE; }
};

