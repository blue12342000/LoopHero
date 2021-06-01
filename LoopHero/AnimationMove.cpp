#include "AnimationMove.h"
#include "AnimationUIController.h"
#include "Utill.h"

void AnimationMove::Exec(AnimVariable& animVar)
{
	if (!vMoveTime.empty())
	{
		animVar.position = animVar.origin + LerpAxis(vMovePoint, animVar.elapsedTime / vMoveTime.back());
	}
}

void AnimationMove::AddEvent(float time, AnimVariable& animVar)
{
}
