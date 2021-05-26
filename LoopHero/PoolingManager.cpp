#include "PoolingManager.h"
#include "GameNode.h"

void PoolingManager::Release()
{
	for (auto& pair : mlLpClass)
	{
		for (GameNode*& node : pair.second)
		{
			delete node;
		}
	}
	mlLpClass.clear();
}

void PoolingManager::AddClass(GameNode* lpClass)
{
	string name = typeid(*lpClass).name();
	auto it = mlLpClass.find(name);
	if (it == mlLpClass.end())
	{
		it = mlLpClass.insert(make_pair(name, list<GameNode*>())).first;
	}
	it->second.push_front(lpClass);
}
