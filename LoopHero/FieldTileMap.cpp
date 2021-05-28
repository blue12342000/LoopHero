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
	for (int y = 0; y < FIELD_TILE_Y; ++y)
	{
		for (int x = 0; x < FIELD_TILE_X; ++x)
		{
			tiles[y][x] = GameObject::Create<FieldTile>(this);
			tiles[y][x]->Init();
			tiles[y][x]->SetPos({ (float)(FIELD_START_X + FIELD_TILE_SIZE * 0.5f + x * FIELD_TILE_SIZE), (float)(FIELD_START_Y + FIELD_TILE_SIZE * 0.5f + y * FIELD_TILE_SIZE) });
			tiles[y][x]->x = x;
			tiles[y][x]->y = y;
			SetRect(&tiles[y][x]->rc, FIELD_START_X + x * FIELD_TILE_SIZE, FIELD_START_Y + y * FIELD_TILE_SIZE, FIELD_START_X + (x + 1) * FIELD_TILE_SIZE, FIELD_START_Y + (y + 1) * FIELD_TILE_SIZE);

			tiles[y][x]->lpTile = nullptr;
			tiles[y][x]->frameX = 0;
			tiles[y][x]->frameY = 0;
			tiles[y][x]->eventCount = 0;

			isPossibleBuild[y][x] = false;
		}
	}

	SetRect(&rc, FIELD_START_X, FIELD_START_Y, FIELD_START_X + FIELD_TILE_SIZE * FIELD_TILE_X, FIELD_START_Y + FIELD_TILE_SIZE * FIELD_TILE_Y);

	lpSelectedTile = nullptr;
	isVisible = true;

	ObserverManager::GetSingleton()->RegisterObserver(this);
	AddOEventHandler("SelectedCard", bind(&FieldTileMap::SelectedCard, this, placeholders::_1));
	AddOEventHandler("DeselectCard", bind(&FieldTileMap::DeselectCard, this, placeholders::_1));
	AddOEventHandler("BattleStart", bind(&FieldTileMap::BattleStart, this, placeholders::_1));
	AddOEventHandler("BattleEnd", bind(&FieldTileMap::BattleEnd, this, placeholders::_1));
}

void FieldTileMap::Release()
{
}

void FieldTileMap::Update(float deltaTime)
{
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
		// 플레이어 이동 테스트
		// 디버그 테스트
		if (mBuildTiles.find("road") != mBuildTiles.end())
		{
			if (lpHero)
			{
				lpHero->Loop(mBuildTiles["road"]);
			}
		}
	}

	if (isVisible)
	{
		for (int i = 0; i < vChilds.size(); ++i)
		{
			vChilds[i]->Update(deltaTime);
		}
	}
}

void FieldTileMap::Render(HDC hdc)
{
	for (int y = 0; y < FIELD_TILE_Y; ++y)
	{
		for (int x = 0; x < FIELD_TILE_X; ++x)
		{
			//Rectangle(hdc, tiles[y][x]->rc.left, tiles[y][x]->rc.top, tiles[y][x]->rc.right, tiles[y][x]->rc.bottom);
			if (tiles[y][x]->lpTile)
			{
				string str = tiles[y][x]->lpTile->name;
				//TextOut(hdc, tiles[y][x]->rc.left, tiles[y][x]->rc.top, str.c_str(), str.length());
				tiles[y][x]->lpTile->mLpImage[TILE_TYPE::TILE]->Render(hdc, (tiles[y][x]->rc.left + tiles[y][x]->rc.right) / 2, tiles[y][x]->rc.bottom, POINT{ tiles[y][x]->frameX, tiles[y][x]->frameY }, IMAGE_ALIGN::MIDDLE_BOTTOM);
				tiles[y][x]->Render(hdc);
			}

			if (isPossibleBuild[y][x])
			{
				//TextOut(hdc, tiles[y][x]->rc.left, tiles[y][x]->rc.top + 30, "O", 1);
				ImageManager::GetSingleton()->FindImage("possible_tile")->Render(hdc, tiles[y][x]->rc.left, tiles[y][x]->rc.top);
			}
		}
	}

	//lpTileTable->Render(hdc);

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
		//TextOut(hdc, 200, 10, lpSelectedTile->name.c_str(), lpSelectedTile->name.length());
	}

	if (lpHero) lpHero->Render(hdc);

	//RenderRectangle(hdc, rc, RGB(255, 0, 255));
}

bool FieldTileMap::BuildTile(int x, int y, Tile* lpTile)
{
	if (lpTile)
	{
		if (isPossibleBuild[y][x])
		{
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
					tiles[y][x]->vChilds.pop_back();
				}
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
			}

			DeselectCard(this);
			return true;
		}
	}
	DeselectCard(this);
	return false;
}

void FieldTileMap::SelectedTileValidation()
{
	if (!lpSelectedTile) return;

	// road일경우 예외처리 road는 단 한사이클만 존재하여야한다.
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
					// 한사이클이 되었으니 더이상 못만듬
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
				if (tiles[tile->y][tile->x]->lpTile->id == "campsite") continue;
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
			// 텅빈상태
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

					// 대각선
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
			// 마지막 타일 인접 && 시작과 끝이 존재하거나 하나도 없을때
			// 무조건 단 하나의 노드만 확인
			if (mBuildTiles.find(lpSelectedTile->vNearTiles[0]) == mBuildTiles.end())
			{
				// 지어진 타일이 없음
				// 어디든 지어도 됨
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
				// 지어진 타일이 있음
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
					// 상하좌우 확인해서 만약 시작과 끝점이 아닌 같은 타일이 만나면 그건 절때 안됨
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
			// 텅비지 않은 상태
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
					//상하좌우
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

					//대각선
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
					// 틀림
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
		// true인 애들의 확인
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
		RemoveChild(&(*lpBattleField));
		lpBattleField->Release();
		lpBattleField = nullptr;
	}
}

void FieldTileMap::SetHero(Hero* lpHero)
{
	this->lpHero = lpHero;
	AddChild(lpHero);

	if (mBuildTiles.find("campsite") != mBuildTiles.end())
	{
		lpHero->SetPos({ FIELD_TILE_SIZE * 0.5f + mBuildTiles["campsite"].back()->GetTilePosX() * FIELD_TILE_SIZE, FIELD_TILE_SIZE * 0.5f + mBuildTiles["campsite"].back()->GetTilePosY() * FIELD_TILE_SIZE });
	}
}

void FieldTileMap::OnClick(EventData& data)
{
	int x = (data.point.x - FIELD_START_X) / FIELD_TILE_SIZE;
	int y = (data.point.y - FIELD_START_Y) / FIELD_TILE_SIZE;
	if (lpSelectedTile)
	{
		BuildTile(x, y, lpSelectedTile);
	}
	else
	{
		if (tiles[y][x]->lpTile && tiles[y][x]->vHistory.front() == "road")
		{
			lpBattleField = GameObject::Create<BattleField>(this);
			lpBattleField->Init();
			lpBattleField->AddUnit(BATTLE_TEAM::LEFT, GameData::GetSingleton()->GetUnit());
			for (auto& lpUnit : tiles[y][x]->vChilds)
			{
				lpBattleField->AddUnit(BATTLE_TEAM::RIGHT, (Unit*)lpUnit);
			}
			ObserverManager::GetSingleton()->Notify("BattleStart", lpBattleField);
		}
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
