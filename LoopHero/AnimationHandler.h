#pragma once
#include "GameNode.h"

struct AnimVariable;
class AnimationHandler : public GameNode
{
public:
	virtual ~AnimationHandler() {}
	virtual void Exec(AnimVariable& animVar) = 0;
	virtual void AddEvent(int index, AnimVariable& animVar) = 0;
	virtual void ReplaceEvent(int index, AnimVariable& animVar) = 0;
	virtual void RemoveEvent(int index) = 0;
};

