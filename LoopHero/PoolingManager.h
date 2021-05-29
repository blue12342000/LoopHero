#pragma once
#include "Singleton.h"
#include <iostream>
#include <map>
#include <string>
#include <list>

using namespace std;

class GameNode;
class PoolingManager : public Singleton<PoolingManager>
{
private:
	map<string, list<GameNode*>> mlLpClass;
	map<string, int> mClassCreateCount;

public:
	void Release();

	template<typename U>
	U* GetClass()
	{
		string name = typeid(U).name();
		
		string log = "Pooling ::: Find " + name + " ::::\n";
		printf(log.c_str());

		auto it = mlLpClass.find(name);
		if (it != mlLpClass.end() && !it->second.empty())
		{
			U* lpClass = (U*)it->second.front();
			it->second.pop_front();
			PrintDebugLog();
			return lpClass;
		}

		if (mClassCreateCount.find(name) == mClassCreateCount.end()) mClassCreateCount.insert(make_pair(name, 1));
		else ++mClassCreateCount[name];
		PrintDebugLog();
		return new U;
	}

	void AddClass(GameNode* lpClass);
	void PrintDebugLog();
};

