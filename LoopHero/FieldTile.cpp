#include "FieldTile.h"
#include "Utill.h"
#include "Unit.h"
#include "Trait.h"

void FieldTile::Init()
{
	isVisible = true;
	SetEventCatch(EVENT_CATCH::BLOCK_PASS);

	angle = 0.0f;
	radius = 15;

	ObserverManager::GetSingleton()->RegisterObserver(this);
	AddOEventHandler("daily_spawn_monster", bind(&FieldTile::DailySpawnMonster, this, placeholders::_1));
}

void FieldTile::Release()
{
}

void FieldTile::Update(float deltaTime)
{
	angle += PI / 2.0f * deltaTime;
	if (angle > PI * 2) angle -= PI * 2;

	SetRect(&rc, pos.x, pos.y, pos.x + 50, pos.y + 50);

	for (int i = 0; i < vChilds.size(); ++i)
	{
		vChilds[i]->SetPos({ 25.0f + cos(angle + PI / 2.0f * i) * radius, 25.0f + sin(angle + PI / 2.0f * i) * radius });
		vChilds[i]->Update(deltaTime);
	}
}

void FieldTile::Render(HDC hdc)
{
	//RenderRectangle(hdc, rc);

	for (int i = 0; i < vChilds.size(); ++i)
	{
		vChilds[i]->Render(hdc);
	}
}

void FieldTile::DailySpawnMonster(ObserverHandler* lpCaller)
{
	if (lpTile && vChilds.size() < 4)
	{
		Unit* lpUnit = Trait::NewUnit(lpTile->spawnUnit);
		AddChild(lpUnit);
		lpUnit->SetPos({ 25.0f + cos(angle + PI / 2.0f * (vChilds.size() - 1)) * radius, 25.0f + sin(angle + PI / 2.0f * (vChilds.size() - 1)) * radius });
		if (rand() % 100 < lpTile->spawnPer)
		{

		}
	}
}

void FieldTile::OnMouseOver(EventData& data)
{
	if (!data.isDragging && lpTile)
	{
		ObserverManager::GetSingleton()->Notify("ViewInfo", this);
	}
}

void FieldTile::OnMouseOut(EventData& data)
{
	ObserverManager::GetSingleton()->Notify("HideInfo", this);
}

string FieldTile::ToString()
{
	if (lpTile) return lpTile->name + "|" + lpTile->desc;
	return "";
}

