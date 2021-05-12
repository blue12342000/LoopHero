#include "TileTable.h"

void TileTable::LoadTileData()
{
	Release();
	DataManager::GetSingleton()->LoadIniFile("Ini/tiles.ini");

	map<string, map<string, string>> mDatas = DataManager::GetSingleton()->GetSingleton()->GetData("Ini/tiles.ini");
	vector<string> vTileData;
	for (auto group : mDatas)
	{
		mLpTiles.insert(make_pair(group.first, new Tile()));
		mLpTiles[group.first]->name = mDatas[group.first]["name"];
		mLpTiles[group.first]->desc = mDatas[group.first]["desc"];
		mLpTiles[group.first]->vEtc = StringSplit(mDatas[group.first]["etc"], '|');
		vTileData = StringSplit(mDatas[group.first]["check_tile"], ',');
		if (!vTileData.empty())
		{
			for (int y = 0; y < 3; ++y)
			{
				for (int x = 0; x < 3; ++x)
				{
					mLpTiles[group.first]->checkTiles[y][x] = stoi(vTileData[y * 3 + x]);
				}
			}
		}
		if (mDatas[group.first].find("self_condition") == mDatas[group.first].end()) mLpTiles[group.first]->selfCondition = -1;
		else mLpTiles[group.first]->selfCondition = stod(mDatas[group.first]["self_condition"]);
		if (mDatas[group.first].find("near_condition") == mDatas[group.first].end()) mLpTiles[group.first]->nearCondition = -1;
		else mLpTiles[group.first]->nearCondition = stod(mDatas[group.first]["near_condition"]);
		mLpTiles[group.first]->vSelfTiles = StringSplit(mDatas[group.first]["self"], ',');
		mLpTiles[group.first]->vNearTiles = StringSplit(mDatas[group.first]["near"], ',');
	}

	vRects.resize(mLpTiles.size());
	int index = 0;
	for (RECT& rc : vRects)
	{
		SetRect(&rc, WINSIZE_WIDTH - 100, 20 + (index++) * 25, WINSIZE_WIDTH, 20 + (index) * 25);
	}
}

void TileTable::Release()
{
	for (auto p : mLpTiles)
	{
		delete p.second;
	}
	mLpTiles.clear();
}

void TileTable::Update()
{
}

void TileTable::Render(HDC hdc)
{
	int index = 0;
	for (auto p : mLpTiles)
	{
		TextOut(hdc, WINSIZE_WIDTH - 100, 20 + (index++) * 25, p.second->name.c_str(), p.second->name.length());
	}
}
