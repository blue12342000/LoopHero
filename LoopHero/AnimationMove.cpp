#include "AnimationMove.h"
#include "AnimationUIController.h"
#include "PoolingManager.h"
#include "Utill.h"

void AnimationMove::Release()
{
	vMoveTime.clear();
	vMovePoint.clear();
	PoolingManager::GetSingleton()->AddClass(this);
}

void AnimationMove::Exec(AnimVariable& animVar)
{
	if (!vMoveTime.empty())
	{
		if (animVar.isLinear)
		{
			animVar.position = animVar.origin + LerpAxis(vMovePoint, (float)animVar.tick / vMoveTime.back());
		}
		else
		{
			if (animVar.sequence < 1)
			{
				animVar.position = animVar.origin + vMovePoint[0];
			}
			else if (animVar.sequence < vMoveTime.size())
			{
				animVar.position = animVar.origin + LerpAxis(vMovePoint[animVar.sequence - 1], vMovePoint[animVar.sequence], (float)(animVar.tick - vMoveTime[animVar.sequence - 1]) / (vMoveTime[animVar.sequence] - vMoveTime[animVar.sequence - 1]));
			}
		}
	}
}

void AnimationMove::AddEvent(int index, AnimVariable& animVar)
{
	vMoveTime.insert(vMoveTime.begin() + index, animVar.tick);
	vMovePoint.insert(vMovePoint.begin() + index, animVar.position);
}

void AnimationMove::ReplaceEvent(int index, AnimVariable& animVar)
{
	if (-1 < index && index < vMoveTime.size())
	{
		vMoveTime[index] = animVar.tick;
		vMovePoint[index] = animVar.position;
	}
}

void AnimationMove::RemoveEvent(int index)
{
	if (-1 < index && index < vMoveTime.size())
	{
		vMoveTime.erase(vMoveTime.begin() + index);
		vMovePoint.erase(vMovePoint.begin() + index);
	}
}

void AnimationMove::ResetEvent()
{
	if (vMoveTime.size() > 1)
	{
		vMoveTime.erase(vMoveTime.begin() + 1, vMoveTime.end());
		vMovePoint.erase(vMovePoint.begin() + 1, vMovePoint.end());
	}
}
