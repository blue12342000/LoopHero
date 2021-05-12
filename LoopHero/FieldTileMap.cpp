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
		// ����Ÿ�Ͽ� ������ �������
		switch (lpSelectedTile->selfCondition)
		{
		case 0:
			// �ֺ�
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
			// Ÿ���߿� �ϳ�
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
			// �ݵ��
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
			// ���� ����̰ų� ������ ����̸鼭 �̾������� ������ �ϳ��� Ÿ�ϸ� ����
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
		// ��óŸ�Ͽ� ���� ������ �����Ұ��
		POINT delta[8] = { {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1} };
		for (int y = 0; y < FIELD_TILE_Y; ++y)
		{
			for (int x = 0; x < FIELD_TILE_X; ++x)
			{
				if (!isPossibleBuild[y][x]) continue;

				// ������ �ִ� Ÿ���� ��ó�� Ȯ��
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
