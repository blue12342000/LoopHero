#pragma once
#include "AnimationHandler.h"
#include <Windows.h>
#include <vector>

using namespace std;

class AnimationMove : public AnimationHandler
{
private:
	vector<int> vMoveTime;
	vector<POINTFLOAT> vMovePoint;

public:
	virtual void Release() override;
	virtual void Exec(AnimVariable& animVar) override;
	virtual void AddEvent(int index, AnimVariable& animVar) override;
	virtual void ReplaceEvent(int index, AnimVariable& animVar) override;
	virtual void RemoveEvent(int index);
	virtual void ResetEvent() override;
};

