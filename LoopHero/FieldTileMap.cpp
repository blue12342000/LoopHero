#include "FieldTileMap.h"
#include "UIHorizontalScroll.h"
#include "UISprite.h"
#include "Card.h"
#include "LoopHero.h"
#include "Image.h"
#include "FieldTile.h"
#include "BattleField.h"
#include "Unit.h"
#include "Trait.h"
#include "Hero.h"
#include "ParticleSystem.h"

TILE_IMAGE_SEQ FieldTileMap::CalTileSeq(int buildX, int buildY, Tile* lpTile)
{
	int data = 0;
	for (int y = 1, digit = 1; y > -2; --y)
	{
		for (int x = 1; x > -2; --x, digit <<= 1)
		{
			if (x == 0 && y == 0) data |= digit;
			if (abs(x + y) != 1) continue;

			if (buildX + x > -1 && buildY + y > -1
				&& buildX + x < FIELD_TILE_X && buildY + y < FIELD_TILE_Y)
			{
				if (tiles[buildY + y][buildX + x]->lpTile)
				{
					if (lpTile->id == "road" && !tiles[buildY + y][buildX + x]->vHistory.empty())
					{
						if (tiles[buildY + y][buildX + x]->vHistory.front() == lpTile->id)
						{
							data |= digit;
						}
					}
					else if (tiles[buildY + y][buildX + x]->lpTile->id == lpTile->id)
					{
						data |= digit;
					}
				}
			}
		}
	}

	return GameData::GetSingleton()->GetTileSeq(data);
}

void FieldTileMap::Init()
{
	SetWorldPos({ (float)FIELD_START_X, (float)FIELD_START_Y });
	SetRect(&rc, FIELD_START_X, FIELD_START_Y, FIELD_START_X + FIELD_TILE_SIZE * FIELD_TILE_X, FIELD_START_Y + FIELD_TILE_SIZE * FIELD_TILE_Y);

	POINTFLOAT worldPos;
	for (int y = 0; y < FIELD_TILE_Y; ++y)
	{
		for (int x = 0; x < FIELD_TILE_X; ++x)
		{
			tiles[y][x] = GameObject::Create<FieldTile>(this);
			tiles[y][x]->SetPos({ (float)(FIELD_TILE_SIZE * 0.5f + x * FIELD_TILE_SIZE), (float)(FIELD_TILE_SIZE * 0.5f + y * FIELD_TILE_SIZE) });
			tiles[y][x]->x = x;
			tiles[y][x]->y = y;

			tiles[y][x]->lpTile = nullptr;
			tiles[y][x]->frameX = 0;
			tiles[y][x]->frameY = 0;
			tiles[y][x]->spawnDelay = 0;

			if (x - 1 > -1)
			{
				tiles[y][x]->lpNearTiles[(int)TILE_DIRECTION::LEFT] = tiles[y][x - 1];
				tiles[y][x - 1]->lpNearTiles[(int)TILE_DIRECTION::RIGHT] = tiles[y][x];
			}
			if (y - 1 > -1)
			{
				tiles[y][x]->lpNearTiles[(int)TILE_DIRECTION::UP] = tiles[y - 1][x];
				tiles[y - 1][x]->lpNearTiles[(int)TILE_DIRECTION::DOWN] = tiles[y][x];
			}

			isPossibleBuild[y][x] = false;
			worldPos = tiles[y][x]->GetWorldPos();
			SetRect(&tiles[y][x]->rc, worldPos.x - FIELD_TILE_SIZE  / 2, worldPos.y - FIELD_TILE_SIZE / 2, worldPos.x + FIELD_TILE_SIZE / 2, worldPos.y + FIELD_TILE_SIZE / 2);

			// ?????? ???????? ???? ???? ?????????? ???? ??????????????.
			GameData::GetSingleton()->AddEventHandler("daily_spawn_monster_" + to_string(x) + "_" + to_string(y), bind(&FieldTile::DailySpawnMonster, tiles[y][x], placeholders::_1));
			GameData::GetSingleton()->AddEventHandler("daily_near_spawn_monster_" + to_string(x) + "_" + to_string(y), bind(&FieldTile::DailyNearSpawnMonster, tiles[y][x], placeholders::_1));
		}
	}

	lpSelectedTile = nullptr;
	isVisible = true;

	lpParticleSystem = PoolingManager::GetSingleton()->GetClass<ParticleSystem>();

	ParticleInfo info;
	info.angle = 0.0f;
	info.deltaAngle = 0.0f;
	info.force = 0.0f;
	info.deltaForce = 0.0f;
	info.spread = PARTICLE_SPREAD::LINE;
	info.movement = PARTICLE_MOVEMENT::STRAIGHT;
	info.type = PATTICLE_TYPE::IMAGE;
	info.align = PARTICLE_ALIGN::MIDDLE_BOTTOM;
	info.lifeCycle = 0.5f;
	lpParticleSystem->Init(info, "tile_appear");

	ParticleManager::GetSingleton()->AddParticleSystem("Tile_ParticleSystem", lpParticleSystem);

	AddEventHandler("SelectedCard", bind(&FieldTileMap::SelectedCard, this, placeholders::_1));
	AddEventHandler("DeselectCard", bind(&FieldTileMap::DeselectCard, this, placeholders::_1));
	AddEventHandler("BattleStart", bind(&FieldTileMap::BattleStart, this, placeholders::_1));
	AddEventHandler("BattleEnd", bind(&FieldTileMap::BattleEnd, this, placeholders::_1));
}

void FieldTileMap::Release()
{
	mBuildTiles.clear();
	lpSelectedTile = nullptr;
	lpHero->SetParent(nullptr);

	lpParticleSystem->Release();
	ParticleManager::GetSingleton()->RemoveParticleSystem("Tile_ParticleSystem");
	GameObject::Release();
	GameData::GetSingleton()->ClearEventHandler();

	ZeroMemory(tiles, sizeof(FieldTile*) * FIELD_TILE_X * FIELD_TILE_Y);
	ZeroMemory(isPossibleBuild, sizeof(bool) * FIELD_TILE_X * FIELD_TILE_Y);
}

void FieldTileMap::Update(float deltaTime)
{
	lpParticleSystem->Update(deltaTime);

	if (KeyManager::GetSingleton()->IsKeyOnceDown('T'))
	{
		if (lpSelectedTile)
		{
			DeselectCard(this);
		}
		else
		{
			lpSelectedTile = GameData::GetSingleton()->GetTile("road");
			if (lpSelectedTile) SelectedTileValidation();
		}
	}

	if (KeyManager::GetSingleton()->IsKeyOnceDown(VK_SPACE))
	{
		// ???????? ???? ??????
		// ?????? ??????
		if (mBuildTiles.find("road") != mBuildTiles.end())
		{
			if (lpHero)
			{
				lpHero->Loop(mBuildTiles["road"]);
			}
		}
	}

	GameObject::Update(deltaTime);
}

void FieldTileMap::Render(HDC hdc)
{
	for (int y = 0; y < FIELD_TILE_Y; ++y)
	{
		for (int x = 0; x < FIELD_TILE_X; ++x)
		{
			if (tiles[y][x]->lpTile)
			{
				//string str = tiles[y][x]->lpTile->name;
				//Rectangle(hdc, tiles[y][x]->rc.left, tiles[y][x]->rc.top, tiles[y][x]->rc.right, tiles[y][x]->rc.bottom);
				tiles[y][x]->Render(hdc);
			}

			if (isPossibleBuild[y][x])
			{
				ImageManager::GetSingleton()->FindImage("possible_tile")->Render(hdc, tiles[y][x]->rc.left, tiles[y][x]->rc.top);
			}
		}
	}

	if (lpSelectedTile)
	{
		POINT mPos = KeyManager::GetSingleton()->GetMousePoint();
		int x = (mPos.x - FIELD_START_X) / FIELD_TILE_SIZE;
		int y = (mPos.y - FIELD_START_Y) / FIELD_TILE_SIZE;
		if (isPossibleBuild[y][x])
		{
			lpSelectedTile->mLpImage[TILE_TYPE::SELECT]->Render(hdc, (tiles[y][x]->rc.left + tiles[y][x]->rc.right) / 2, tiles[y][x]->rc.bottom, POINT{ tiles[y][x]->frameX, tiles[y][x]->frameY }, IMAGE_ALIGN::MIDDLE_BOTTOM);
		}
		else
		{
			lpSelectedTile->mLpImage[TILE_TYPE::SELECT]->Render(hdc, mPos.x, mPos.y, POINT{ 0, 0 }, IMAGE_ALIGN::CENTER);
		}
	}

	if (lpHero) lpHero->Render(hdc);

	lpParticleSystem->Render(hdc);
}

bool FieldTileMap::BuildTile(int x, int y, Tile* lpTile)
{
	if (lpTile)
	{
		if (isPossibleBuild[y][x])
		{
			tiles[y][x]->type = TILE_TYPE::WHITE;
			tiles[y][x]->spawnDelay = lpTile->spawnDelay;

			tiles[y][x]->ClearEventHandler();
			for (const string& eventKey : lpTile->vEventKey)
			{
				tiles[y][x]->AddEventHandler(eventKey, GameData::GetSingleton()->GetEventHandler(eventKey + "_" + to_string(x) + "_" + to_string(y)));
			}

			if (lpTile->id == "oblivion")
			{
				if (!tiles[y][x]->vHistory.empty())
				{
					if (tiles[y][x]->vHistory.front() == "road")
					{
						tiles[y][x]->lpTile = GameData::GetSingleton()->GetTile("road");
					}
					else
					{
						tiles[y][x]->lpTile = nullptr;
					}
					tiles[y][x]->vHistory.erase(tiles[y][x]->vHistory.begin() + 1, tiles[y][x]->vHistory.end());
				}
				while (!tiles[y][x]->vChilds.empty())
				{
					tiles[y][x]->vChilds.back()->Release();
				}
				ParticleManager::GetSingleton()->SpreadParticle("Tile_ParticleSystem", POINTFLOAT{ tiles[y][x]->GetWorldPos().x, tiles[y][x]->GetWorldPos().y + FIELD_TILE_SIZE / 2 }, POINT{ FIELD_TILE_SIZE, WINSIZE_HEIGHT }, bind(&FieldTileMap::BuildTileFinish, this, x, y));
				ObserverManager::GetSingleton()->Notify("IncreaseBossTimer", this);
				DeselectCard(this);
				return true;
			}

			tiles[y][x]->lpTile = lpTile;
			if (tiles[y][x]->vHistory.empty())
			{
				tiles[y][x]->frameY = (int)CalTileSeq(x, y, lpTile) % lpTile->mLpImage[TILE_TYPE::TILE]->GetMaxFrameY();

				if (lpTile->id == "road")
				{
					for (int dy = 1; dy > -2; --dy)
					{
						for (int dx = 1; dx > -2; --dx)
						{
							if (abs(dx + dy) != 1) continue;

							if (dx + x > -1 && dy + y > -1
								&& dx + x < FIELD_TILE_X && dy + y < FIELD_TILE_Y)
							{
								if (!tiles[dy + y][dx + x]->lpTile) continue;
								tiles[dy + y][dx + x]->frameY = (int)CalTileSeq(dx + x, dy + y, tiles[dy + y][dx + x]->lpTile) % tiles[dy + y][dx + x]->lpTile->mLpImage[TILE_TYPE::TILE]->GetMaxFrameY();
							}
						}
					}
				}
			}

			tiles[y][x]->vHistory.push_back(lpTile->id);
			auto it = mBuildTiles.find(lpTile->id);
			if (it == mBuildTiles.end())
			{
				mBuildTiles.insert(make_pair(lpTile->id, vector<FieldTile*>()));
			}
			mBuildTiles[lpTile->id].push_back(tiles[y][x]);

			if (lpTile->id == "road")
			{
				if (mBuildTiles[lpTile->id].size() == 1)
				{
					tiles[y][x]->lpTile = GameData::GetSingleton()->GetTile("campsite");
					mBuildTiles[tiles[y][x]->lpTile->id].push_back(tiles[y][x]);
					tiles[y][x]->vHistory.push_back("campsite");
				}
				if (lpHero->IsLoop()) lpHero->Loop(mBuildTiles["road"]);
			}
			ParticleManager::GetSingleton()->SpreadParticle("Tile_ParticleSystem", POINTFLOAT{ tiles[y][x]->GetWorldPos().x, tiles[y][x]->GetWorldPos().y + FIELD_TILE_SIZE / 2 }, POINT{ FIELD_TILE_SIZE, WINSIZE_HEIGHT }, bind(&FieldTileMap::BuildTileFinish, this, x, y));
			ObserverManager::GetSingleton()->Notify("IncreaseBossTimer", this);
			DeselectCard(this);
			return true;
		}
	}
	DeselectCard(this);
	return false;
}

void FieldTileMap::BuildTileFinish(int x, int y)
{
	tiles[y][x]->type = TILE_TYPE::TILE;
}

void FieldTileMap::SelectedTileValidation()
{
	if (!lpSelectedTile) return;

	// road?????? ???????? road?? ?? ?????????? ??????????????.
	if (lpSelectedTile->id == "road")
	{
		if (mBuildTiles.find(lpSelectedTile->id) != mBuildTiles.end())
		{
			if (mBuildTiles[lpSelectedTile->id].size() > 2)
			{
				FieldTile* lpFrontTile = mBuildTiles[lpSelectedTile->id].front();
				FieldTile* lpBackTile = mBuildTiles[lpSelectedTile->id].back();
				if (abs(lpFrontTile->x - lpBackTile->x) + abs(lpFrontTile->y - lpBackTile->y) == 1)
				{
					// ?????????? ???????? ?????? ??????
					return;
				}
			}
		}
	}

	if (lpSelectedTile->id == "oblivion")
	{
		for (int y = 0; y < FIELD_TILE_Y; ++y)
		{
			for (int x = 0; x < FIELD_TILE_X; ++x)
			{
				isPossibleBuild[y][x] = false;
			}
		}

		for (auto& pair : mBuildTiles)
		{
			for (const auto& tile : pair.second)
			{
				if (!tiles[tile->y][tile->x]->lpTile || tiles[tile->y][tile->x]->lpTile->id == "campsite") continue;
				isPossibleBuild[tile->y][tile->x] = true;
			}
		}
		return;
	}

	map<FieldTile*, int> mCheckMap;
	if (lpSelectedTile->nearCondition != -1)
	{
		if (lpSelectedTile->nearCondition == 0)
		{
			// ????????
			for (int y = 0; y < FIELD_TILE_Y; ++y)
			{
				for (int x = 0; x < FIELD_TILE_X; ++x)
				{
					isPossibleBuild[y][x] = true;
				}
			}

			for (auto pair : mBuildTiles)
			{
				for (FieldTile* lpFieldTile : pair.second)
				{
					isPossibleBuild[lpFieldTile->y][lpFieldTile->x] = false;

					if (lpSelectedTile->checkTiles[2][1] && lpFieldTile->y - 1 > -1)
					{
						isPossibleBuild[lpFieldTile->y - 1][lpFieldTile->x] = false;
					}
					if (lpSelectedTile->checkTiles[1][2] && lpFieldTile->x - 1 > -1)
					{
						isPossibleBuild[lpFieldTile->y][lpFieldTile->x - 1] = false;
					}
					if (lpSelectedTile->checkTiles[0][1] && lpFieldTile->y + 1 < FIELD_TILE_Y)
					{
						isPossibleBuild[lpFieldTile->y + 1][lpFieldTile->x] = false;
					}
					if (lpSelectedTile->checkTiles[1][0] && lpFieldTile->x + 1 < FIELD_TILE_X)
					{
						isPossibleBuild[lpFieldTile->y][lpFieldTile->x + 1] = false;
					}

					// ??????
					if (lpSelectedTile->checkTiles[0][0] && lpFieldTile->y - 1 > -1 && lpFieldTile->x - 1 > -1)
					{
						isPossibleBuild[lpFieldTile->y - 1][lpFieldTile->x - 1] = false;
					}
					if (lpSelectedTile->checkTiles[2][2] && lpFieldTile->y + 1 < FIELD_TILE_Y && lpFieldTile->x + 1 < FIELD_TILE_X)
					{
						isPossibleBuild[lpFieldTile->y + 1][lpFieldTile->x + 1] = false;
					}
					if (lpSelectedTile->checkTiles[0][2] && lpFieldTile->y - 1 > -1 && lpFieldTile->x + 1 < FIELD_TILE_X)
					{
						isPossibleBuild[lpFieldTile->y - 1][lpFieldTile->x + 1] = false;
					}
					if (lpSelectedTile->checkTiles[2][0] && lpFieldTile->y + 1 < FIELD_TILE_Y && lpFieldTile->x - 1 > -1)
					{
						isPossibleBuild[lpFieldTile->y + 1][lpFieldTile->x - 1] = false;
					}
				}
			}
		}
		else if (lpSelectedTile->nearCondition == 99)
		{
			// ?????? ???? ???? && ?????? ???? ?????????? ?????? ??????
			// ?????? ?? ?????? ?????? ????
			if (mBuildTiles.find(lpSelectedTile->vNearTiles[0]) == mBuildTiles.end())
			{
				// ?????? ?????? ????
				// ?????? ?????? ??
				for (int y = 0; y < FIELD_TILE_Y; ++y)
				{
					for (int x = 0; x < FIELD_TILE_X; ++x)
					{
						isPossibleBuild[y][x] = (tiles[y][x]->lpTile == nullptr);
					}
				}
			}
			else
			{
				// ?????? ?????? ????
				for (int y = 0; y < FIELD_TILE_Y; ++y)
				{
					for (int x = 0; x < FIELD_TILE_X; ++x)
					{
						isPossibleBuild[y][x] = false;
					}
				}

				FieldTile* lpFieldTile = mBuildTiles[lpSelectedTile->vNearTiles[0]].back();
				if (lpSelectedTile->checkTiles[2][1] && lpFieldTile->y - 1 > -1)
				{
					isPossibleBuild[lpFieldTile->y - 1][lpFieldTile->x] = true;
					// ???????? ???????? ???? ?????? ?????? ???? ???? ?????? ?????? ???? ???? ????
					if (lpFieldTile->y - 2 > -1)
					{
						if (tiles[lpFieldTile->y - 2][lpFieldTile->x]->lpTile == lpSelectedTile && tiles[lpFieldTile->y - 2][lpFieldTile->x] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& tiles[lpFieldTile->y - 2][lpFieldTile->x] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y - 1][lpFieldTile->x] = false;
						}
					}
					if (lpFieldTile->x - 1 > -1)
					{
						if (tiles[lpFieldTile->y - 1][lpFieldTile->x - 1]->lpTile == lpSelectedTile && tiles[lpFieldTile->y - 1][lpFieldTile->x - 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& tiles[lpFieldTile->y - 1][lpFieldTile->x - 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y - 1][lpFieldTile->x] = false;
						}
					}
					if (lpFieldTile->x + 1 < FIELD_TILE_X)
					{
						if (tiles[lpFieldTile->y - 1][lpFieldTile->x + 1]->lpTile == lpSelectedTile && tiles[lpFieldTile->y - 1][lpFieldTile->x + 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& tiles[lpFieldTile->y - 1][lpFieldTile->x + 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y - 1][lpFieldTile->x] = false;
						}
					}
				}
				if (lpSelectedTile->checkTiles[1][2] && lpFieldTile->x - 1 > -1)
				{
					isPossibleBuild[lpFieldTile->y][lpFieldTile->x - 1] = true;
					if (lpFieldTile->y - 1 > -1)
					{
						if (tiles[lpFieldTile->y - 1][lpFieldTile->x - 1]->lpTile == lpSelectedTile && tiles[lpFieldTile->y - 1][lpFieldTile->x - 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& tiles[lpFieldTile->y - 1][lpFieldTile->x - 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y][lpFieldTile->x - 1] = false;
						}
					}
					if (lpFieldTile->y + 1 < FIELD_TILE_Y)
					{
						if (tiles[lpFieldTile->y + 1][lpFieldTile->x - 1]->lpTile == lpSelectedTile && tiles[lpFieldTile->y + 1][lpFieldTile->x - 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& tiles[lpFieldTile->y + 1][lpFieldTile->x - 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y][lpFieldTile->x - 1] = false;
						}
					}
					if (lpFieldTile->x - 2 > -1)
					{
						if (tiles[lpFieldTile->y][lpFieldTile->x - 2]->lpTile == lpSelectedTile && tiles[lpFieldTile->y][lpFieldTile->x - 2] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& tiles[lpFieldTile->y][lpFieldTile->x - 2] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y][lpFieldTile->x - 1] = false;
						}
					}
				}
				if (lpSelectedTile->checkTiles[0][1] && lpFieldTile->y + 1 < FIELD_TILE_Y)
				{
					isPossibleBuild[lpFieldTile->y + 1][lpFieldTile->x] = true;
					if (lpFieldTile->y + 2 < FIELD_TILE_Y)
					{
						if (tiles[lpFieldTile->y + 2][lpFieldTile->x]->lpTile == lpSelectedTile && tiles[lpFieldTile->y + 2][lpFieldTile->x] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& tiles[lpFieldTile->y + 2][lpFieldTile->x] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y + 1][lpFieldTile->x] = false;
						}
					}
					if (lpFieldTile->x - 1 > -1)
					{
						if (tiles[lpFieldTile->y + 1][lpFieldTile->x - 1]->lpTile == lpSelectedTile && tiles[lpFieldTile->y + 1][lpFieldTile->x - 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& tiles[lpFieldTile->y + 1][lpFieldTile->x - 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y + 1][lpFieldTile->x] = false;
						}
					}
					if (lpFieldTile->x + 1 < FIELD_TILE_X)
					{
						if (tiles[lpFieldTile->y + 1][lpFieldTile->x + 1]->lpTile == lpSelectedTile && tiles[lpFieldTile->y + 1][lpFieldTile->x + 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& tiles[lpFieldTile->y + 1][lpFieldTile->x + 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y + 1][lpFieldTile->x] = false;
						}
					}
				}
				if (lpSelectedTile->checkTiles[1][0] && lpFieldTile->x + 1 < FIELD_TILE_X)
				{
					isPossibleBuild[lpFieldTile->y][lpFieldTile->x + 1] = true;
					if (lpFieldTile->y - 1 > -1)
					{
						if (tiles[lpFieldTile->y - 1][lpFieldTile->x + 1]->lpTile == lpSelectedTile && tiles[lpFieldTile->y - 1][lpFieldTile->x + 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& tiles[lpFieldTile->y - 1][lpFieldTile->x + 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y][lpFieldTile->x + 1] = false;
						}
					}
					if (lpFieldTile->y + 1 < FIELD_TILE_Y)
					{
						if (tiles[lpFieldTile->y + 1][lpFieldTile->x + 1]->lpTile == lpSelectedTile && tiles[lpFieldTile->y + 1][lpFieldTile->x + 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& tiles[lpFieldTile->y + 1][lpFieldTile->x + 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y][lpFieldTile->x + 1] = false;
						}
					}
					if (lpFieldTile->x + 2 < FIELD_TILE_X)
					{
						if (tiles[lpFieldTile->y][lpFieldTile->x + 2]->lpTile == lpSelectedTile && tiles[lpFieldTile->y][lpFieldTile->x + 2] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& tiles[lpFieldTile->y][lpFieldTile->x  + 2] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y][lpFieldTile->x + 1] = false;
						}
					}
				}

			}
		}
		else
		{
			// ?????? ???? ????
			for (int y = 0; y < FIELD_TILE_Y; ++y)
			{
				for (int x = 0; x < FIELD_TILE_X; ++x)
				{
					isPossibleBuild[y][x] = false;
				}
			}

			for (string tileKey : lpSelectedTile->vNearTiles)
			{
				if (mBuildTiles.find(tileKey) == mBuildTiles.end()) continue;

				for (FieldTile* lpFieldTile : mBuildTiles[tileKey])
				{
					//????????
					if (lpSelectedTile->checkTiles[2][1] && lpFieldTile->y - 1 > -1)
					{
						auto it = mCheckMap.find(tiles[lpFieldTile->y - 1][lpFieldTile->x]);
						if (!tiles[lpFieldTile->y - 1][lpFieldTile->x]->lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(tiles[lpFieldTile->y - 1][lpFieldTile->x], 1));
							else ++(it->second);
						}
					}
					if (lpSelectedTile->checkTiles[1][2] && lpFieldTile->x - 1 > -1)
					{
						auto it = mCheckMap.find(tiles[lpFieldTile->y][lpFieldTile->x - 1]);
						if (!tiles[lpFieldTile->y][lpFieldTile->x - 1]->lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(tiles[lpFieldTile->y][lpFieldTile->x - 1], 1));
							else ++(it->second);
						}
					}
					if (lpSelectedTile->checkTiles[0][1] && lpFieldTile->y + 1 < FIELD_TILE_Y)
					{
						auto it = mCheckMap.find(tiles[lpFieldTile->y + 1][lpFieldTile->x]);
						if (!tiles[lpFieldTile->y + 1][lpFieldTile->x]->lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(tiles[lpFieldTile->y + 1][lpFieldTile->x], 1));
							else ++(it->second);
						}
					}
					if (lpSelectedTile->checkTiles[1][0] && lpFieldTile->x + 1 < FIELD_TILE_X)
					{
						auto it = mCheckMap.find(tiles[lpFieldTile->y][lpFieldTile->x + 1]);
						if (!tiles[lpFieldTile->y][lpFieldTile->x + 1]->lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(tiles[lpFieldTile->y][lpFieldTile->x + 1], 1));
							else ++(it->second);
						}
					}

					//??????
					if (lpSelectedTile->checkTiles[0][0] && lpFieldTile->y - 1 > -1 && lpFieldTile->x - 1 > -1)
					{
						auto it = mCheckMap.find(tiles[lpFieldTile->y - 1][lpFieldTile->x - 1]);
						if (!tiles[lpFieldTile->y - 1][lpFieldTile->x - 1]->lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(tiles[lpFieldTile->y - 1][lpFieldTile->x - 1], 1));
							else ++(it->second);
						}
					}
					if (lpSelectedTile->checkTiles[2][2] && lpFieldTile->y + 1 < FIELD_TILE_Y && lpFieldTile->x + 1 < FIELD_TILE_X)
					{
						auto it = mCheckMap.find(tiles[lpFieldTile->y + 1][lpFieldTile->x + 1]);
						if (!tiles[lpFieldTile->y + 1][lpFieldTile->x + 1]->lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(tiles[lpFieldTile->y + 1][lpFieldTile->x + 1], 1));
							else ++(it->second);
						}
					}
					if (lpSelectedTile->checkTiles[0][2] && lpFieldTile->y - 1 > -1 && lpFieldTile->x + 1 < FIELD_TILE_X)
					{
						auto it = mCheckMap.find(tiles[lpFieldTile->y - 1][lpFieldTile->x + 1]);
						if (!tiles[lpFieldTile->y - 1][lpFieldTile->x + 1]->lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(tiles[lpFieldTile->y - 1][lpFieldTile->x + 1], 1));
							else ++(it->second);
						}
					}
					if (lpSelectedTile->checkTiles[2][0] && lpFieldTile->y + 1 < FIELD_TILE_Y && lpFieldTile->x - 1 > -1)
					{
						auto it = mCheckMap.find(tiles[lpFieldTile->y + 1][lpFieldTile->x - 1]);
						if (!tiles[lpFieldTile->y + 1][lpFieldTile->x - 1]->lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(tiles[lpFieldTile->y + 1][lpFieldTile->x - 1], 1));
							else ++(it->second);
						}
					}
				}
			}

			switch (lpSelectedTile->nearCondition)
			{
			case 1:
				for (auto pair : mCheckMap)
				{
					if (pair.second > 0)
					{
						isPossibleBuild[pair.first->y][pair.first->x] = true;
					}
				}
				break;
			case 2:
				for (auto pair : mCheckMap)
				{
					// ????
					if (pair.second == lpSelectedTile->vNearTiles.size())
					{
						isPossibleBuild[pair.first->y][pair.first->x] = true;
					}
				}
				break;
			case 3:
				for (auto pair : mCheckMap)
				{
					if (pair.second > 0)
					{
						isPossibleBuild[pair.first->y][pair.first->x] = true;
					}
				}

				for (int y = 0; y < FIELD_TILE_Y; ++y)
				{
					for (int x = 0; x < FIELD_TILE_X; ++x)
					{
						isPossibleBuild[y][x] = !isPossibleBuild[y][x];
					}
				}
				break;
			}
		}

	}
	else
	{
		for (int y = 0; y < FIELD_TILE_Y; ++y)
		{
			for (int x = 0; x < FIELD_TILE_X; ++x)
			{
				isPossibleBuild[y][x] = true;
			}
		}
	}

	if (lpSelectedTile->selfCondition != -1)
	{
		// true?? ?????? ????
		for (int y = 0; y < FIELD_TILE_Y; ++y)
		{
			for (int x = 0; x < FIELD_TILE_X; ++x)
			{
				if (isPossibleBuild[y][x])
				{
					switch (lpSelectedTile->selfCondition)
					{
					case 0:
						isPossibleBuild[y][x] = (tiles[y][x]->lpTile == nullptr);
						break;
					case 1:
						if (!tiles[y][x]->lpTile)
						{
							isPossibleBuild[y][x] = false;
						}
						else
						{
							isPossibleBuild[y][x] = false;
							for (string tileKey : lpSelectedTile->vSelfTiles)
							{
								if (tiles[y][x]->lpTile->id == tileKey)
								{
									isPossibleBuild[y][x] = true;
									break;
								}
							}
						}
						break;
					case 2:
						if (!tiles[y][x]->lpTile || lpSelectedTile->vSelfTiles.size() > 1)
						{
							isPossibleBuild[y][x] = false;
						}
						else
						{
							isPossibleBuild[y][x] = false;
							for (string tileKey : lpSelectedTile->vSelfTiles)
							{
								if (tiles[y][x]->lpTile->id == tileKey)
								{
									isPossibleBuild[y][x] = true;
								}
							}
						}
						break;
					}
				}
			}
		}
	}
}

void FieldTileMap::SelectedCard(ObserverHandler* lpObserver)
{
	if (typeid(*lpObserver) == typeid(Card))
	{
		Card* lpCard = (Card*)(lpObserver);
		lpSelectedTile = GameData::GetSingleton()->GetTile(lpCard->GetTileKey());
		SelectedTileValidation();
	}
}

void FieldTileMap::DeselectCard(ObserverHandler* lpObserver)
{
	lpSelectedTile = nullptr;
	for (int y = 0; y < FIELD_TILE_Y; ++y)
	{
		for (int x = 0; x < FIELD_TILE_X; ++x)
		{
			isPossibleBuild[y][x] = false;
		}
	}
}

void FieldTileMap::SetTile(int x, int y, Tile* lpTile)
{
	tiles[y][x]->lpTile = lpTile;
}

void FieldTileMap::BattleStart(ObserverHandler* lpCaller)
{
	if (typeid(*lpCaller) == typeid(BattleField))
	{
		lpBattleField = (BattleField*)lpCaller;
		AddChild(lpBattleField);
	}
}

void FieldTileMap::BattleEnd(ObserverHandler* lpCaller)
{
	if (lpBattleField)
	{
		RemoveChild(lpBattleField);
		lpBattleField = nullptr;
	}
}

void FieldTileMap::SetHero(Hero* lpHero)
{
	this->lpHero = lpHero;
	AddChild(lpHero);

	//if (mBuildTiles.find("campsite") != mBuildTiles.end())
	//{
	//	lpHero->SetPos({ FIELD_TILE_SIZE * 0.5f + mBuildTiles["campsite"].back()->GetTilePosX() * FIELD_TILE_SIZE, FIELD_TILE_SIZE * 0.5f + mBuildTiles["campsite"].back()->GetTilePosY() * FIELD_TILE_SIZE });
	//}
}

void FieldTileMap::OnClick(EventData& data)
{
	int x = (data.point.x - FIELD_START_X) / FIELD_TILE_SIZE;
	int y = (data.point.y - FIELD_START_Y) / FIELD_TILE_SIZE;
	if (lpSelectedTile)
	{
		BuildTile(x, y, lpSelectedTile);
	}
}

void FieldTileMap::OnMouseEnter(EventData& data)
{
	if (data.lpDragTarget)
	{
		if (typeid(*data.lpDragTarget) == typeid(UIHorizontalScroll))
		{
			UIHorizontalScroll* lpHandCard = (UIHorizontalScroll*)data.lpDragTarget;
			GameUI* lpObject = lpHandCard->GetSelectedObject();
			if (lpObject && typeid(*lpObject) == typeid(UISprite))
			{
				UISprite* lpSprite = (UISprite*)lpObject;
				Card* lpObject = (Card*)lpSprite->GetGameObject();
				if (lpObject) SelectedCard(lpObject);
			}
		}
	}
}

void FieldTileMap::OnMouseOver(EventData& data)
{
	//MessageBox(g_hWnd, "OnMouseOver", "", MB_OK);
}

void FieldTileMap::OnMouseOut(EventData& data)
{
	//MessageBox(g_hWnd, "OnMouseOut", "", MB_OK);
	if (lpSelectedTile) DeselectCard(this);
}

void FieldTileMap::OnDrop(EventData& data)
{
	if (data.lpDragTarget)
	{
		if (typeid(*data.lpDragTarget) == typeid(UISprite))
		{
			if (lpSelectedTile)
			{
				int x = (data.point.x - FIELD_START_X) / FIELD_TILE_SIZE;
				int y = (data.point.y - FIELD_START_Y) / FIELD_TILE_SIZE;
				if (BuildTile(x, y, lpSelectedTile))
				{
					ObserverManager::GetSingleton()->Notify("UseCard", (UISprite*)data.lpDragTarget);
				}
			}
		}
	}
}
