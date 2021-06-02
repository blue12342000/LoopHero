#pragma once
#include "AnimationHandler.h"
#include <Windows.h>
#include <vector>

using namespace std;

class AnimationMove : public AnimationHandler
{
private:
	bool isLinear;
	vector<int> vMoveTime;
	vector<POINTFLOAT> vMovePoint;

public:
	virtual void Exec(AnimVariable& animVar) final;
	virtual void AddEvent(int index, AnimVariable& animVar) final;
	virtual void ReplaceEvent(int index, AnimVariable& animVar) final;
	virtual void RemoveEvent(int index) final;
};

