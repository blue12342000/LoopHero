#include "Hero.h"
#include "Unit.h"
#include "Trait.h"
#include "FieldTileMap.h"
#include "FieldTile.h"
#include "BattleField.h"
#include "Utill.h"

void Hero::Init()
{
	state == HERO_STATE::IDLE;
	ObserverManager::GetSingleton()->RegisterObserver(this);
	AddOEventHandler("BattleEnd", bind(&Hero::BattleEnd, this, placeholders::_1));

	SetEventCatch(EVENT_CATCH::PASS);
}

void Hero::Release()
{
	if (lpUnit)
	{
		lpUnit->Release();
		lpUnit = nullptr;
	}
	ObserverManager::GetSingleton()->RemoveObserver(this);
	RemoveOEventHandler("BattleEnd");
	PoolingManager::GetSingleton()->AddClass(this);
}

void Hero::Update(float deltaTime)
{
	if (state == HERO_STATE::MOVE)
	{
		FieldTile* lpFieldTile = *tileIter.iter;
		POINTFLOAT target = lpFieldTile->GetPos();
		//target.x += FIELD_TILE_SIZE + 0.5f;
		//target.y += FIELD_TILE_SIZE + 0.5f;
		float angle = atan2(target.y - pos.y, target.x - pos.x);
		pos.x += cosf(angle) * FIELD_TILE_SIZE * deltaTime;
		pos.y += sinf(angle) * FIELD_TILE_SIZE * deltaTime;

		SetRect(&rc, pos.x, pos.y, pos.x, pos.y);

		float diff = pow(pos.x - target.x, 2) + (pos.y - target.y, 2);
		if (diff < 10)
		{
			if (lpFieldTile->GetChildCount() > 0)
			{
				state = HERO_STATE::BATTLE;
				BattleField* lpBattleField = GameObject::Create<BattleField>();
				lpBattleField->Init();
				lpBattleField->AddUnit(BATTLE_TEAM::LEFT, lpUnit);
				for (auto& lpEnemy : lpFieldTile->GetChilds())
				{
					lpBattleField->AddUnit(BATTLE_TEAM::RIGHT, (Unit*)lpEnemy);
				}
				ObserverManager::GetSingleton()->Notify("BattleStart", lpBattleField);
			}
			pos = target;
			SetRect(&rc, pos.x, pos.y, pos.x, pos.y);
			++tileIter;
		}
	}

	if (lpUnit)
	{
		lpUnit->Update(deltaTime);
	}
}

void Hero::Render(HDC hdc)
{
	if (lpUnit) lpUnit->Render(hdc);
	Ellipse(hdc, pos.x - 5, pos.y - 5, pos.x + 5, pos.y + 5);
}

void Hero::BattleEnd(ObserverHandler* lpCaller)
{
	if (state == HERO_STATE::BATTLE)
	{
		state = HERO_STATE::MOVE;
	}
}

void Hero::Loop(vector<FieldTile*>& vMovePath)
{
	state = HERO_STATE::MOVE;
	tileIter = CycleIterator(vMovePath);
	SetPos((*tileIter.iter)->GetPos());

	SetRect(&rc, pos.x, pos.y, pos.x, pos.y);
	lpUnit->SetPos({ 0.0f, 0.0f });
}

void Hero::NewHero(string id)
{
	lpUnit = Trait::NewUnit(id);
	AddChild(lpUnit);
}
