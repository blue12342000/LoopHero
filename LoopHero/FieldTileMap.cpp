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

	if (lpSelectedTile->nearCondition != -1)
	{
		// 근처타일에 대한 조건이 존재할경우
		POINT delta[8] = { {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1} };
		for (int y = 0; y < FIELD_TILE_Y; ++y)
		{
			for (int x = 0; x < FIELD_TILE_X; ++x)
			{
				if (!isPossibleBuild[y][x]) continue;

				// 지을수 있는 타일의 근처만 확인
				switch (lpSelectedTile->nearCondition)
				{
				case 0:
					for (int ny = y - 1, dy = 0; dy < 3; ++ny, ++dy)
					{
						if (ny < 0 || ny >= FIELD_TILE_Y) continue;
						for (int nx = x - 1, dx = 0; dy < 3; ++nx, ++dx)
						{
							if (nx < 0 || nx >= FIELD_TILE_X) continue;
							if (dy == 1 && dx == 1) continue;

							isPossibleBuild[y][x] = (tiles[ny][nx].lpTile == nullptr);
						}
						if (!isPossibleBuild[y][x]) break;
					}
					break;
				case 1:
					for (int ny = y - 1, dy = 0; dy < 3; ++ny, ++dy)
					{
						if (ny < 0 || ny >= FIELD_TILE_Y) continue;
						for (int nx = x - 1, dx = 0; dy < 3; ++nx, ++dx)
						{
							if (nx < 0 || nx >= FIELD_TILE_X) continue;
							if (dy == 1 && dx == 1) continue;

							isPossibleBuild[y][x] = (tiles[ny][nx].lpTile == nullptr);
						}
						if (!isPossibleBuild[y][x]) break;
					}
					break;
				case 2:
					break;
				case 99:
					break;
				}
			}
		}
	}
}
