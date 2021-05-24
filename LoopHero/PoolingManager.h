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

public:
	void Release();

	template<typename U>
	U* GetClass()
	{
		string name = typeid(U*).name();
		auto it = mlLpClass.find(name);
		if (it == mlLpClass.end())
		{
			return nullptr;
		}
		else
		{
			U* lpClass = (U*)it->second.front();
			it->second.pop_front();
			return lpClass;
		}
	}

	void AddClass(GameNode* lpClass);
};

