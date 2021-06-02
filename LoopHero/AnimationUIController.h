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
	bool isLinear;
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

	set<int> sAnimTick;
	set<int>::iterator tickIter;
	vector<AnimVariable> vAnimVars;

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

	void ResetEvent();
	void AddEventTime(float time);
	void AddEventTick(int timeTick);

	vector<AnimVariable> GetAnimVariables();

	inline vector<int> GetEventTicks() { return vector<int>(sAnimTick.begin(), sAnimTick.end()); }
	inline bool IsEventExist(float time) { return sAnimTick.find((int)(time / tickScale + FLT_EPSILON)) != sAnimTick.end(); }
	inline bool IsEventExist(int tick) { return sAnimTick.find(tick) != sAnimTick.end(); }
	inline float GetElapsedTime() { return animVar.elapsedTime; }
	inline bool IsPlay() { return isPlay; }
	inline bool IsLinear() { return animVar.isLinear; }
	inline void ToggleLinear() { animVar.isLinear = !animVar.isLinear; }
};

