#pragma once
#include "GameNode.h"
#include "ParticleSystem.h"
#include <Windows.h>
#include <functional>

#define GRAVITY_SCALA 20

using namespace std;

struct Transform
{
	POINTFLOAT pos;
	POINTFLOAT velocity;
	int width;
	int height;
};

class Animation;
class Text;
class Particle : public GameNode
{
private:
	bool isActive;
	ParticleInfo info;
	Animation* lpAnimation;
	Text* lpText;

	Transform transform;
	float elapsedTime;

	function<void()> callBack;

public:
	void Init(const ParticleInfo& info, Transform transform, Animation* lpGameNode);
	void Init(const ParticleInfo& info, Transform transform, Text* lpGameNode);
	void Release();
	void Update(float deltaTime);
	void Render(HDC hdc);

	inline void SetCallBack(function<void()> callBack) { this->callBack = callBack; }
	inline bool IsActive() { return isActive; }
	inline bool IsDective() { return !isActive; }
};

