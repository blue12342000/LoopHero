#pragma once
#include "AnimationHandler.h"
#include <Windows.h>
#include <vector>

using namespace std;

class AnimationMove : public AnimationHandler
{
private:
	vector<float> vMoveTime;
	vector<POINTFLOAT> vMovePoint;

public:
	virtual void Exec(AnimVariable& animVar) final;
	virtual void AddEvent(float time, AnimVariable& animVar) final;
};

