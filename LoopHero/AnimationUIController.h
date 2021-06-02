#pragma once
#include "GameNode.h"
#include "PoolingManager.h"
#include <Windows.h>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <functional>

using namespace std;

struct AnimVariable
{
	POINTFLOAT origin;
	POINTFLOAT position;
	int tick;
	float elapsedTime;

	int sequence;
};

struct AnimTick
{
	int tick;
	vector<string> vAnimHandle;
};

class GameUI;
class AnimationHandler;
class AnimationUIController : public GameNode
{
private:
	bool isPlay;
	bool isLoop;
	bool isRootChange;
	float tickScale;
	GameUI* lpTarget;
	map<string, AnimationHandler*> mAnimHandler;
	AnimVariable animVar;

	//set<AnimTick, function<bool(AnimTick,AnimTick)>> sAnimTick;
	set<int> sAnimTick;
	set<int>::iterator tickIter;

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
			mAnimHandler.insert(make_pair(name, lpAnimHandler));
			//sAnimTick.begin()->vAnimHandle.push_back(name);
		}
		else
		{
			PoolingManager::GetSingleton()->AddClass(lpAnimHandler);
			lpAnimHandler = (T*)mAnimHandler[name];
		}
		return lpAnimHandler;
	}

	void Play();
	void Resume();
	void Stop();

	void AddEventTime(float time);
	void AddEventTick(int timeTick);

	inline vector<int> GetEventTicks() { return vector<int>(sAnimTick.begin(), sAnimTick.end()); }
	inline bool IsEventExist(int tick) { return sAnimTick.find(tick) != sAnimTick.end(); }
	inline float GetElapsedTime() { return animVar.elapsedTime; }
	inline bool IsPlay() { return isPlay; }
};

