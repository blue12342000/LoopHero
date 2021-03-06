#include "FieldTile.h"
#include "Utill.h"
#include "Unit.h"
#include "Trait.h"
#include "FieldTileMap.h"
#include "Image.h"

void FieldTile::Init()
{
	isVisible = true;
	SetEventCatch(EVENT_CATCH::BLOCK_PASS);

	angle = 0.0f;
	radius = 15;
}

void FieldTile::Release()
{
	type = TILE_TYPE::WHITE;
	direction = TILE_DIRECTION::NONE;
	x = -1;
	y = -1;
	lpTile = nullptr;
	ZeroMemory(lpNearTiles, sizeof(FieldTile*) * (int)TILE_DIRECTION::NONE);
	vHistory.clear();

	GameObject::Release();
}

void FieldTile::Update(float deltaTime)
{
	// 해당 타일애서 생성된 몹들 회전
	angle += PI / 2.0f * deltaTime;
	if (angle > PI * 2) angle -= PI * 2;

	for (int i = 0; i < vChilds.size(); ++i)
	{
		vChilds[i]->SetPos({ cos(angle + PI / 2.0f * i) * radius, sin(angle + PI / 2.0f * i) * radius });
		vChilds[i]->Update(deltaTime);
	}
}

void FieldTile::Render(HDC hdc)
{
	//RenderRectangle(hdc, rc);
	if (lpTile)
	{
		lpTile->mLpImage[type]->Render(hdc, GetWorldPos().x, rc.bottom, POINT{ frameX, frameY }, IMAGE_ALIGN::MIDDLE_BOTTOM);
	}

	GameObject::Render(hdc);
}

void FieldTile::DailySpawnMonster(ObserverHandler* lpCaller)
{
	if (lpTile && vChilds.size() < 4 && !lpTile->spawnUnit.empty())
	{
		--spawnDelay;
		if (spawnDelay <= 0)
		{
			if (rand() % 100 < lpTile->spawnPer)
			{
				// 확률
				Unit* lpUnit = Trait::NewUnit(lpTile->spawnUnit);
				AddChild(lpUnit);
				lpUnit->SetPos({ 25.0f + cos(angle + PI / 2.0f * (vChilds.size() - 1)) * radius, 25.0f + sin(angle + PI / 2.0f * (vChilds.size() - 1)) * radius });
			}

			spawnDelay = lpTile->spawnDelay;
		}
	}
}

void FieldTile::DailyNearSpawnMonster(ObserverHandler* lpCaller)
{
	if (!lpTile) return;

	--spawnDelay;
	if (spawnDelay <= 0)
	{
		if (rand() % 100 < lpTile->spawnPer)
		{
			vector<FieldTile*> vFieldTiles;
			vFieldTiles.insert(vFieldTiles.end(), begin(lpNearTiles), end(lpNearTiles));
			random_device rd;
			mt19937 g(rd());
			shuffle(vFieldTiles.begin(), vFieldTiles.end(), g);
				
			for (const auto& lpFieldTile : vFieldTiles)
			{
				if (!lpFieldTile->vHistory.empty()
					&& lpFieldTile->vHistory.front() == "road"
					&& lpFieldTile->vChilds.size() < 4)
				{
					// 확률
					Unit* lpUnit = Trait::NewUnit(lpTile->spawnUnit);
					lpFieldTile->AddChild(lpUnit);
					lpUnit->SetPos({ 25.0f + cos(angle + PI / 2.0f * (lpFieldTile->vChilds.size() - 1)) * radius, 25.0f + sin(angle + PI / 2.0f * (lpFieldTile->vChilds.size() - 1)) * radius });
					break;
				}
			}
			spawnDelay = lpTile->spawnDelay;
		}
	}
}

void FieldTile::ClearMonster()
{
	while (!vChilds.empty())
	{
		RemoveChild(0);
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

