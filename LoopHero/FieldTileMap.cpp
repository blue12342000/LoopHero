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
	lpTileTable->LoadTileData();

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
		if (KeyManager::GetSingleton()->IsKeyDownStay(VK_LBUTTON))
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
					tiles[y][x].vHistory.push_back(lpSelectedTile->name);
					auto it = mBuildTiles.find(lpSelectedTile->name);
					if (it == mBuildTiles.end())
					{
						mBuildTiles.insert(make_pair(lpSelectedTile->name, vector<FieldTile*>()));
					}
					mBuildTiles[lpSelectedTile->name].push_back(&tiles[y][x]);
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
				if (KeyManager::GetSingleton()->IsKeyDownOne(VK_LBUTTON))
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
				TextOut(hdc, tiles[y][x].rc.left, tiles[y][x].rc.top + 15, "O", 1);
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

			for (string tileKey : lpSelectedTile->vNearTiles)
			{
				if (mBuildTiles.find(tileKey) == mBuildTiles.end()) continue;

				for (FieldTile* lpFieldTile : mBuildTiles[tileKey])
				{
					isPossibleBuild[lpFieldTile->y][lpFieldTile->x] = false;

					if (lpSelectedTile->checkTiles[2][1] && lpFieldTile->y - 1 > 0)
					{
						isPossibleBuild[lpFieldTile->y - 1][lpFieldTile->x] = false;
					}
					if (lpSelectedTile->checkTiles[1][2] && lpFieldTile->x - 1 > 0)
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
					if (lpSelectedTile->checkTiles[0][0] && lpFieldTile->y - 1 > 0 && lpFieldTile->x - 1 > 0)
					{
						isPossibleBuild[lpFieldTile->y - 1][lpFieldTile->x - 1] = false;
					}
					if (lpSelectedTile->checkTiles[2][2] && lpFieldTile->y + 1 < FIELD_TILE_Y && lpFieldTile->x + 1 < FIELD_TILE_X)
					{
						isPossibleBuild[lpFieldTile->y + 1][lpFieldTile->x + 1] = false;
					}
					if (lpSelectedTile->checkTiles[0][2] && lpFieldTile->y - 1 > 0 && lpFieldTile->x + 1 < FIELD_TILE_X)
					{
						isPossibleBuild[lpFieldTile->y = 1][lpFieldTile->x + 1] = false;
					}
					if (lpSelectedTile->checkTiles[2][0] && lpFieldTile->y + 1 < FIELD_TILE_Y && lpFieldTile->x - 1 > 0)
					{
						isPossibleBuild[lpFieldTile->y + 1][lpFieldTile->x - 1] = false;
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
					if (lpSelectedTile->checkTiles[2][1] && lpFieldTile->y - 1 > 0)
					{
						auto it = mCheckMap.find(&tiles[lpFieldTile->y - 1][lpFieldTile->x]);
						if (!it->first->lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(&tiles[lpFieldTile->y-1][lpFieldTile->x], 1));
							else ++(it->second);
						}
					}
					if (lpSelectedTile->checkTiles[1][2] && lpFieldTile->x - 1 > 0)
					{
						auto it = mCheckMap.find(&tiles[lpFieldTile->y][lpFieldTile->x - 1]);
						if (!it->first->lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(&tiles[lpFieldTile->y][lpFieldTile->x - 1], 1));
							else ++(it->second);
						}
					}
					if (lpSelectedTile->checkTiles[0][1] && lpFieldTile->y + 1 < FIELD_TILE_Y)
					{
						auto it = mCheckMap.find(&tiles[lpFieldTile->y + 1][lpFieldTile->x]);
						if (!it->first->lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(&tiles[lpFieldTile->y + 1][lpFieldTile->x], 1));
							else ++(it->second);
						}
					}
					if (lpSelectedTile->checkTiles[1][0] && lpFieldTile->x + 1 < FIELD_TILE_X)
					{
						auto it = mCheckMap.find(&tiles[lpFieldTile->y][lpFieldTile->x + 1]);
						if (!it->first->lpTile)
						{
							if (it == mCheckMap.end()) mCheckMap.insert(make_pair(&tiles[lpFieldTile->y][lpFieldTile->x + 1], 1));
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
					if (pair.second == lpSelectedTile->vNearTiles.size())
					{
						isPossibleBuild[pair.first->y][pair.first->x] = true;
					}
				}
				break;
			}
		}
	}

	if (lpSelectedTile->selfCondition != -1)
	{
		// 본인타일에 조건이 있을경우
		switch (lpSelectedTile->selfCondition)
		{
		case 0:
			// 텅빈
			for (int y = 0; y < FIELD_TILE_Y; ++y)
			{
				for (int x = 0; x < FIELD_TILE_X; ++x)
				{
					isPossibleBuild[y][x] = true;
				}
			}

			for (string tileKey : lpSelectedTile->vSelfTiles)
			{
				auto it = mBuildTiles.find(tileKey);
				if (it == mBuildTiles.end()) continue;

				for (auto lpFieldTile : it->second)
				{
					isPossibleBuild[lpFieldTile->y][lpFieldTile->x] = false;
				}
			}
			break;
		case 1:
			// 타일중에 하나
			for (int y = 0; y < FIELD_TILE_Y; ++y)
			{
				for (int x = 0; x < FIELD_TILE_X; ++x)
				{
					isPossibleBuild[y][x] = false;
				}
			}

			for (string tileKey : lpSelectedTile->vSelfTiles)
			{
				auto it = mBuildTiles.find(tileKey);
				if (it == mBuildTiles.end()) continue;

				for (auto lpFieldTile : it->second)
				{
					isPossibleBuild[lpFieldTile->y][lpFieldTile->x] = true;
				}
			}
			break;
		case 2:
			// 반드시
			for (int y = 0; y < FIELD_TILE_Y; ++y)
			{
				for (int x = 0; x < FIELD_TILE_X; ++x)
				{
					isPossibleBuild[y][x] = false;
				}
			}
			if (lpSelectedTile->vSelfTiles.size() == 1)
			{
				auto it = mBuildTiles.find(lpSelectedTile->vSelfTiles[0]);
				if (it != mBuildTiles.end())
				{
					for (auto lpFieldTile : it->second)
					{
						isPossibleBuild[lpFieldTile->y][lpFieldTile->x] = true;
					}
				}
			}
			break;
		case 99:
			// 최초 노드이거나 마지막 노드이면서 이어져있지 않을때 하나의 타일만 가능
			for (int y = 0; y < FIELD_TILE_Y; ++y)
			{
				for (int x = 0; x < FIELD_TILE_X; ++x)
				{
					isPossibleBuild[y][x] = false;
				}
			}
			auto it = mBuildTiles.find(lpSelectedTile->vSelfTiles[0]);
			if (it != mBuildTiles.end()) isPossibleBuild[it->second.back()->y][it->second.back()->x] = true;
			break;
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

}
