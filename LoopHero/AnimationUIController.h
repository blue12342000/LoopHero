#pragma once
#include "GameNode.h"
#include "PoolingManager.h"
#include <Windows.h>
#include <vector>
#include <string>
#include <map>

using namespace std;

struct AnimVariable
{
	POINTFLOAT origin;
	POINTFLOAT position;
	float elapsedTime;
};

class GameUI;
class AnimationHandler;
class AnimationUIController : public GameNode
{
private:
	bool isPlay;
	bool isLoop;
	bool isRootChange;
	GameUI* lpTarget;
	map<string, AnimationHandler*> mAnimHandler;
	AnimVariable animVar;

public:
	void Init(GameUI* lpTarget);
	void Release();
	void Update(float deltaTime);
	void Render(HDC hdc);

	template<typename T>
	inline T* AddAnimationHandler()
	{
		T* lpAnimHandler = PoolingManager::GetSingleton()->GetClass<T>();
		string name = typeid(T).name();
		auto it = mAnimHandler.find(name);
		if (it == mAnimHandler.end())
		{
			mAnimHandler.insert(make_pair(typeid(T).name(), lpAnimHandler));
		}
		else
		{
			PoolingManager::GetSingleton()->AddClass(lpAnimHandler);
			lpAnimHandler = it->second;
		}
		return lpAnimHandler;
	}

	void Play();
	void Resume();
	void Stop();
};

