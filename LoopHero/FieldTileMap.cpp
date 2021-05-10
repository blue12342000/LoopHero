#include "FieldTileMap.h"

void FieldTileMap::Init()
{
	for (int y = 0; y < FIELD_TILE_Y; ++y)
	{
		for (int x = 0; x < FIELD_TILE_X; ++x)
		{
			tiles[y][x].x = x;
			tiles[y][x].y = y;
			SetRect(&tiles[y][x].rc, FIELD_START_X + x * FIELD_TILE_SIZE, FIELD_START_Y + y * FIELD_TILE_SIZE, FIELD_START_X + (x + 1) * FIELD_TILE_SIZE, FIELD_START_Y + (y + 1) * FIELD_TILE_SIZE);

			tiles[y][x].frameX = 0;
			tiles[y][x].frameY = 0;
		}
	}

	SetRect(&rc, FIELD_START_X, FIELD_START_Y, FIELD_START_X + FIELD_TILE_SIZE * FIELD_TILE_X, FIELD_START_Y + FIELD_TILE_SIZE * FIELD_TILE_Y);
}

void FieldTileMap::Release()
{
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
}

void FieldTileMap::Render(HDC hdc)
{
	for (int y = 0; y < FIELD_TILE_Y; ++y)
	{
		for (int x = 0; x < FIELD_TILE_X; ++x)
		{
			Rectangle(hdc, tiles[y][x].rc.left, tiles[y][x].rc.top, tiles[y][x].rc.right, tiles[y][x].rc.bottom);
			string str = to_string(tiles[y][x].frameX) + "_" + to_string(tiles[y][x].frameY);
			TextOut(hdc, tiles[y][x].rc.left, tiles[y][x].rc.top, str.c_str(), str.length());
		}
	}
}