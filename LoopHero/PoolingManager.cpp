#include "PoolingManager.h"
#include "GameNode.h"

void PoolingManager::Release()
{
	for (auto& pair : mlLpClass)
	{
		for (GameNode* node : pair.second)
		{
			delete node;
		}
	}
	mlLpClass.clear();
}

void PoolingManager::AddClass(GameNode* lpClass)
{
	string name = typeid(*lpClass).name();

	string log = "Pooling ::: Return " + name + " ::::\n";
	printf(log.c_str());

	auto it = mlLpClass.find(name);
	if (it == mlLpClass.end())
	{
		it = mlLpClass.insert(make_pair(name, list<GameNode*>())).first;
	}
	it->second.push_front(lpClass);

	PrintDebugLog();
}

void PoolingManager::PrintDebugLog()
{
	string log = "=======================================================\n";
	printf(log.c_str());
	const auto& endClass = mlLpClass.end();
	int deactiveCount = 0;
	for (const auto& pair : mClassCreateCount)
	{
		const auto& it = mlLpClass.find(pair.first);
		if (it == endClass) deactiveCount = 0;
		else deactiveCount = mlLpClass[pair.first].size();
		log = pair.first + ":\t CreateCount : " + to_string(pair.second) + "\t/ DeactiveCount : " + to_string(deactiveCount) + "\n";
		printf(log.c_str());
	}
	log = "=======================================================\n";
	printf(log.c_str());
}
