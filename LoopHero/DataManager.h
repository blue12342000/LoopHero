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
	void LoadIniFile(string filePath, string tag);
	void SaveIniFile(string filePath, string tag);

	inline void ReplaceData(string tag, string section, map<string, string> mData)
	{
		auto fit = mDatas.find(tag);
		if (fit == mDatas.end())
		{
			fit = mDatas.insert(make_pair(tag, map<string, map<string, string>>())).first;
		}
		auto git = fit->second.find(section);
		if (git == fit->second.end())
		{
			fit->second.insert(make_pair(section, move(mData)));
		}
		else
		{
			git->second.clear();
			git->second.insert(mData.begin(), mData.end());
		}
	}

	inline string GetData(string tag, string group, string key)
	{
		auto fit = mDatas.find(tag);
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

	inline map<string, map<string, string>> GetData(string tag)
	{
		auto fit = mDatas.find(tag);
		if (fit != mDatas.end())
		{
			return fit->second;
		}
		return map<string, map<string, string>>();
	}

	inline map<string, string> GetData(string tag, string section)
	{
		auto fit = mDatas.find(tag);
		if (fit != mDatas.end())
		{
			auto sit = fit->second.find(section);
			if (sit != fit->second.end())
			{
				return sit->second;
			}
		}
		return map<string, string>();
	}
};

