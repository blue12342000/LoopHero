#include "Hero.h"
#include "Unit.h"
#include "Trait.h"
#include "FieldTileMap.h"
#include "FieldTile.h"
#include "BattleField.h"
#include "Utill.h"

void Hero::Init()
{
	state = HERO_STATE::IDLE;
	SetEventCatch(EVENT_CATCH::PASS);
	AddEventHandler("Resume_Loop", bind(&Hero::Move, this, placeholders::_1));
}

void Hero::Release()
{
	if (lpUnit)
	{
		lpUnit->Release();
		lpUnit = nullptr;
	}
	GameObject::Release();
}

void Hero::Update(float deltaTime)
{
	if (state == HERO_STATE::MOVE)
	{
		FieldTile* lpFieldTile = *tileIter.iter;
		POINTFLOAT target = lpFieldTile->GetPos();
		float angle = atan2(target.y - pos.y, target.x - pos.x);
		pos.x += cosf(angle) * FIELD_TILE_SIZE * deltaTime;
		pos.y += sinf(angle) * FIELD_TILE_SIZE * deltaTime;

		SetRect(&rc, pos.x, pos.y, pos.x, pos.y);

		float diff = pow(pos.x - target.x, 2) + pow(pos.y - target.y, 2);
		if (diff < 5)
		{
			if (lpFieldTile->GetChildCount() > 0)
			{
				state = HERO_STATE::BATTLE;
				BattleField* lpBattleField = GameObject::Create<BattleField>();
				lpBattleField->AddUnit(BATTLE_TEAM::LEFT, lpUnit);
				lpBattleField->SetFieldTile(lpFieldTile);
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
	Ellipse(hdc, GetWorldPos().x - 5, GetWorldPos().y - 5, GetWorldPos().x + 5, GetWorldPos().y + 5);
}

void Hero::Move(ObserverHandler* lpCaller)
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
