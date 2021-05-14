#include "TileTable.h"

void TileTable::Init()
{
	LoadTileData();

	vRects.resize(mLpTiles.size());
	int index = 0;
	for (RECT& rc : vRects)
	{
		SetRect(&rc, WINSIZE_WIDTH - 100, 20 + (index) * 25, WINSIZE_WIDTH, 20 + (index + 1) * 25);
		++index;
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
	SetBkMode(hdc, OPAQUE);
	int index = 0;
	for (auto p : mLpTiles)
	{
		TextOut(hdc, WINSIZE_WIDTH - 100, 20 + (index++) * 25, p.second->name.c_str(), p.second->name.length());
		
	}
	SetBkMode(hdc, TRANSPARENT);

	//for (int i = 0; i < vRects.size(); ++i)
	//{
	//	Rectangle(hdc, vRects[i].left, vRects[i].top, vRects[i].right, vRects[i].bottom);
	//}
}

void TileTable::LoadTileData()
{
	DataManager::GetSingleton()->LoadIniFile("Ini/tiles.ini", "tiles");

	map<string, map<string, string>> mDatas = DataManager::GetSingleton()->GetSingleton()->GetData("tiles");
	vector<string> vTileData;
	for (auto group : mDatas)
	{
		mLpTiles.insert(make_pair(group.first, new Tile()));
		mLpTiles[group.first]->id = group.first;
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
}