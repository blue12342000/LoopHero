#include "FieldTileMap.h"
#include "TileTable.h"

void FieldTileMap::Init()
{
	for (int y = 0; y < FIELD_TILE_Y; ++y)
	{
		for (int x = 0; x < FIELD_TILE_X; ++x)
		{
			tiles[y][x].x = x;
			tiles[y][x].y = y;
			SetRect(&tiles[y][x].rc, FIELD_START_X + x * FIELD_TILE_SIZE, FIELD_START_Y + y * FIELD_TILE_SIZE, FIELD_START_X + (x + 1) * FIELD_TILE_SIZE, FIELD_START_Y + (y + 1) * FIELD_TILE_SIZE);

			tiles[y][x].lpTile = nullptr;
			tiles[y][x].frameX = 0;
			tiles[y][x].frameY = 0;
			tiles[y][x].eventCount = 0;

			isPossibleBuild[y][x] = false;
		}
	}

	SetRect(&rc, FIELD_START_X, FIELD_START_Y, FIELD_START_X + FIELD_TILE_SIZE * FIELD_TILE_X, FIELD_START_Y + FIELD_TILE_SIZE * FIELD_TILE_Y);

	lpTileTable = new TileTable();
	lpTileTable->Init();

	lpSelectedTile = nullptr;
}

void FieldTileMap::Release()
{
	if (lpTileTable)
	{
		lpTileTable->Release();
		delete lpTileTable;
		lpTileTable = nullptr;
	}
}

void FieldTileMap::Update(float deltaTime)
{
	if (PtInRect(&rc, KeyManager::GetSingleton()->GetMousePoint()))
	{
		if (KeyManager::GetSingleton()->IsKeyStayDown(VK_LBUTTON))
		{
			POINT mPoint = KeyManager::GetSingleton()->GetMousePoint();
			int x = (mPoint.x - FIELD_START_X) / FIELD_TILE_SIZE;
			int y = (mPoint.y - FIELD_START_Y) / FIELD_TILE_SIZE;

			tiles[y][x].frameX += 1;
			tiles[y][x].frameY += 1;

			if (lpSelectedTile)
			{
				if (isPossibleBuild[y][x])
				{
					tiles[y][x].lpTile = lpSelectedTile;
					tiles[y][x].vHistory.push_back(lpSelectedTile->id);
					auto it = mBuildTiles.find(lpSelectedTile->id);
					if (it == mBuildTiles.end())
					{
						mBuildTiles.insert(make_pair(lpSelectedTile->id, vector<FieldTile*>()));
					}
					mBuildTiles[lpSelectedTile->id].push_back(&tiles[y][x]);
					SelectedTileValidation();
				}
			}
		}
	}
	else
	{
		int index = 0;
		for (RECT trc : lpTileTable->GetVRects())
		{
			if (PtInRect(&trc, KeyManager::GetSingleton()->GetMousePoint()))
			{
				if (KeyManager::GetSingleton()->IsKeyOnceDown(VK_LBUTTON))
				{
					lpSelectedTile = lpTileTable->GetTile(index);
					SelectedTileValidation();
				}
				break;
			}
			++index;
		}
	}
}

void FieldTileMap::Render(HDC hdc)
{
	for (int y = 0; y < FIELD_TILE_Y; ++y)
	{
		for (int x = 0; x < FIELD_TILE_X; ++x)
		{
			Rectangle(hdc, tiles[y][x].rc.left, tiles[y][x].rc.top, tiles[y][x].rc.right, tiles[y][x].rc.bottom);
			if (isPossibleBuild[y][x])
			{
				TextOut(hdc, tiles[y][x].rc.left, tiles[y][x].rc.top + 30, "O", 1);
			}
			if (tiles[y][x].lpTile)
			{
				string str = tiles[y][x].lpTile->name;
				TextOut(hdc, tiles[y][x].rc.left, tiles[y][x].rc.top, str.c_str(), str.length());
			}

		}
	}

	lpTileTable->Render(hdc);

	if (lpSelectedTile)
	{
		TextOut(hdc, 200, 10, lpSelectedTile->name.c_str(), lpSelectedTile->name.length());
	}
}

void FieldTileMap::SelectedTileValidation()
{
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
						isPossibleBuild[y][x] = (tiles[y][x].lpTile == nullptr);
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
						if (tiles[lpFieldTile->y - 2][lpFieldTile->x].lpTile == lpSelectedTile && &tiles[lpFieldTile->y - 2][lpFieldTile->x] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& &tiles[lpFieldTile->y - 2][lpFieldTile->x] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y - 1][lpFieldTile->x] = false;
						}
					}
					if (lpFieldTile->x - 1 > -1)
					{
						if (tiles[lpFieldTile->y - 1][lpFieldTile->x - 1].lpTile == lpSelectedTile && &tiles[lpFieldTile->y - 1][lpFieldTile->x - 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& &tiles[lpFieldTile->y - 1][lpFieldTile->x - 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y - 1][lpFieldTile->x] = false;
						}
					}
					if (lpFieldTile->x + 1 < FIELD_TILE_X)
					{
						if (tiles[lpFieldTile->y - 1][lpFieldTile->x + 1].lpTile == lpSelectedTile && &tiles[lpFieldTile->y - 1][lpFieldTile->x + 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& &tiles[lpFieldTile->y - 1][lpFieldTile->x + 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
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
						if (tiles[lpFieldTile->y - 1][lpFieldTile->x - 1].lpTile == lpSelectedTile && &tiles[lpFieldTile->y - 1][lpFieldTile->x - 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& &tiles[lpFieldTile->y - 1][lpFieldTile->x - 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y][lpFieldTile->x - 1] = false;
						}
					}
					if (lpFieldTile->y + 1 < FIELD_TILE_Y)
					{
						if (tiles[lpFieldTile->y + 1][lpFieldTile->x - 1].lpTile == lpSelectedTile && &tiles[lpFieldTile->y + 1][lpFieldTile->x - 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& &tiles[lpFieldTile->y + 1][lpFieldTile->x - 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y][lpFieldTile->x - 1] = false;
						}
					}
					if (lpFieldTile->x - 2 > -1)
					{
						if (tiles[lpFieldTile->y][lpFieldTile->x - 2].lpTile == lpSelectedTile && &tiles[lpFieldTile->y][lpFieldTile->x - 2] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& &tiles[lpFieldTile->y][lpFieldTile->x - 2] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y][lpFieldTile->x - 1] = false;
						}
					}
				}
				if (lpSelectedTile->checkTiles[0][1] && lpFieldTile->y + 1 < FIELD_TILE_Y)
				{
					isPossibleBuild[lpFieldTile->y + 1][lpFieldTile->x] = true;
					if (lpFieldTile->y + 2 < FIELD_START_Y)
					{
						if (tiles[lpFieldTile->y + 2][lpFieldTile->x].lpTile == lpSelectedTile && &tiles[lpFieldTile->y + 2][lpFieldTile->x] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& &tiles[lpFieldTile->y + 2][lpFieldTile->x] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y + 1][lpFieldTile->x] = false;
						}
					}
					if (lpFieldTile->x - 1 > -1)
					{
						if (tiles[lpFieldTile->y + 1][lpFieldTile->x - 1].lpTile == lpSelectedTile && &tiles[lpFieldTile->y + 1][lpFieldTile->x - 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& &tiles[lpFieldTile->y + 1][lpFieldTile->x - 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y + 1][lpFieldTile->x] = false;
						}
					}
					if (lpFieldTile->x + 1 < FIELD_TILE_X)
					{
						if (tiles[lpFieldTile->y + 1][lpFieldTile->x + 1].lpTile == lpSelectedTile && &tiles[lpFieldTile->y + 1][lpFieldTile->x + 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& &tiles[lpFieldTile->y + 1][lpFieldTile->x + 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
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
						if (tiles[lpFieldTile->y - 1][lpFieldTile->x + 1].lpTile == lpSelectedTile && &tiles[lpFieldTile->y - 1][lpFieldTile->x + 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& &tiles[lpFieldTile->y - 1][lpFieldTile->x + 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y][lpFieldTile->x + 1] = false;
						}
					}
					if (lpFieldTile->y + 1 < FIELD_TILE_Y)
					{
						if (tiles[lpFieldTile->y + 1][lpFieldTile->x + 1].lpTile == lpSelectedTile && &tiles[lpFieldTile->y + 1][lpFieldTile->x + 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& &tiles[lpFieldTile->y + 1][lpFieldTile->x + 1] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
						{
							isPossibleBuild[lpFieldTile->y][lpFieldTile->x + 1] = false;
						}
					}
					if (lpFieldTile->x + 2 < FIELD_TILE_X)
					{
						if (tiles[lpFieldTile->y][lpFieldTile->x + 2].lpTile == lpSelectedTile && &tiles[lpFieldTile->y][lpFieldTile->x + 2] != mBuildTiles[lpSelectedTile->vNearTiles[0]].back()
							&& &tiles[lpFieldTile->y][lpFieldTile->x  + 2] != mBuildTiles[lpSelectedTile->vNearTiles[0]].front())
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
						auto it = mCheckMap.find(&tiles[lpFieldTile->y - 1][lpFieldTile->x]);
						if (!tiles[lpFieldTile->y - 1][lpFieldTile->x].lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(&tiles[lpFieldTile->y - 1][lpFieldTile->x], 1));
							else ++(it->second);
						}
					}
					if (lpSelectedTile->checkTiles[1][2] && lpFieldTile->x - 1 > -1)
					{
						auto it = mCheckMap.find(&tiles[lpFieldTile->y][lpFieldTile->x - 1]);
						if (!tiles[lpFieldTile->y][lpFieldTile->x - 1].lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(&tiles[lpFieldTile->y][lpFieldTile->x - 1], 1));
							else ++(it->second);
						}
					}
					if (lpSelectedTile->checkTiles[0][1] && lpFieldTile->y + 1 < FIELD_TILE_Y)
					{
						auto it = mCheckMap.find(&tiles[lpFieldTile->y + 1][lpFieldTile->x]);
						if (!tiles[lpFieldTile->y + 1][lpFieldTile->x].lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(&tiles[lpFieldTile->y + 1][lpFieldTile->x], 1));
							else ++(it->second);
						}
					}
					if (lpSelectedTile->checkTiles[1][0] && lpFieldTile->x + 1 < FIELD_TILE_X)
					{
						auto it = mCheckMap.find(&tiles[lpFieldTile->y][lpFieldTile->x + 1]);
						if (!tiles[lpFieldTile->y][lpFieldTile->x + 1].lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(&tiles[lpFieldTile->y][lpFieldTile->x + 1], 1));
							else ++(it->second);
						}
					}

					//대각선
					if (lpSelectedTile->checkTiles[0][0] && lpFieldTile->y - 1 > -1 && lpFieldTile->x - 1 > -1)
					{
						auto it = mCheckMap.find(&tiles[lpFieldTile->y - 1][lpFieldTile->x - 1]);
						if (!tiles[lpFieldTile->y - 1][lpFieldTile->x - 1].lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(&tiles[lpFieldTile->y - 1][lpFieldTile->x - 1], 1));
							else ++(it->second);
						}
					}
					if (lpSelectedTile->checkTiles[2][2] && lpFieldTile->y + 1 < FIELD_TILE_Y && lpFieldTile->x + 1 < FIELD_TILE_X)
					{
						auto it = mCheckMap.find(&tiles[lpFieldTile->y + 1][lpFieldTile->x + 1]);
						if (!tiles[lpFieldTile->y + 1][lpFieldTile->x + 1].lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(&tiles[lpFieldTile->y + 1][lpFieldTile->x + 1], 1));
							else ++(it->second);
						}
					}
					if (lpSelectedTile->checkTiles[0][2] && lpFieldTile->y - 1 > -1 && lpFieldTile->x + 1 < FIELD_TILE_X)
					{
						auto it = mCheckMap.find(&tiles[lpFieldTile->y - 1][lpFieldTile->x + 1]);
						if (!tiles[lpFieldTile->y - 1][lpFieldTile->x + 1].lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(&tiles[lpFieldTile->y - 1][lpFieldTile->x + 1], 1));
							else ++(it->second);
						}
					}
					if (lpSelectedTile->checkTiles[2][0] && lpFieldTile->y + 1 < FIELD_TILE_Y && lpFieldTile->x - 1 > -1)
					{
						auto it = mCheckMap.find(&tiles[lpFieldTile->y + 1][lpFieldTile->x - 1]);
						if (!tiles[lpFieldTile->y + 1][lpFieldTile->x - 1].lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(&tiles[lpFieldTile->y + 1][lpFieldTile->x - 1], 1));
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
						isPossibleBuild[y][x] = (tiles[y][x].lpTile == nullptr);
						break;
					case 1:
						if (!tiles[y][x].lpTile)
						{
							isPossibleBuild[y][x] = false;
						}
						else
						{
							isPossibleBuild[y][x] = false;
							for (string tileKey : lpSelectedTile->vSelfTiles)
							{
								if (tiles[y][x].lpTile->id == tileKey)
								{
									isPossibleBuild[y][x] = true;
									break;
								}
							}
						}
						break;
					case 2:
						if (!tiles[y][x].lpTile || lpSelectedTile->vSelfTiles.size() > 1)
						{
							isPossibleBuild[y][x] = false;
						}
						else
						{
							isPossibleBuild[y][x] = false;
							for (string tileKey : lpSelectedTile->vSelfTiles)
							{
								if (tiles[y][x].lpTile->id == tileKey)
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
