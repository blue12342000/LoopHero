#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <fstream>
#include <map>
#include <string>

using namespace std;

class DataManager : public Singleton<DataManager>
{
private:
	map<string, map<string, string>> mDatas;

public:
	void LoadIniFile(string filePath);

	inline string GetData(string group, string key) { return (mDatas.find(group) != mDatas.end())?((mDatas[group].find(key) != mDatas[group].end())?mDatas[group][key]:""):""; }
};

