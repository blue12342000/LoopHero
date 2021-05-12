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
	map<string, map<string, map<string, string>>> mDatas;

public:
	void LoadIniFile(string filePath);

	inline string GetData(string filePath, string group, string key)
	{
		auto fit = mDatas.find(filePath);
		if (fit != mDatas.end())
		{
			auto git = fit->second.find(group);
			if (git != fit->second.end())
			{
				auto kit = git->second.find(key);
				if (kit != git->second.end())
				{
					return kit->second;
				}
			}
		}
		return "";
	}

	inline map<string, map<string, string>> GetData(string filePath)
	{
		auto fit = mDatas.find(filePath);
		if (fit != mDatas.end())
		{
			return fit->second;
		}
		return map<string, map<string, string>>();
	}
};

