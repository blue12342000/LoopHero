#include "AnimationUIController.h"
#include "AnimationHandler.h"
#include "AnimationMove.h"
#include "PoolingManager.h"
#include "GameUI.h"

void AnimationUIController::Init(GameUI* lpTarget)
{
	this->lpTarget = lpTarget;
	this->isPlay = false;

	this->tickScale = 0.01f;

	//auto compare = [](AnimTick a, AnimTick b) { return a.tick < b.tick; };
	//sAnimTick = decltype(sAnimTick)(compare);
	AddAnimationHandler<AnimationMove>();
	AddEventTick(0);
}

void AnimationUIController::Release()
{
	lpTarget = nullptr;
	isPlay = false;

	for (const auto& pair : mAnimHandler)
	{
		PoolingManager::GetSingleton()->AddClass(pair.second);
	}
	mAnimHandler.clear();
	sAnimTick.clear();
	PoolingManager::GetSingleton()->AddClass(this);
}

void AnimationUIController::Update(float deltaTime)
{
	if (isPlay)
	{
		if (*tickIter < animVar.tick)
		{
			++tickIter;
			++animVar.sequence;
		}

		for (const auto& pair : mAnimHandler)
		{
			pair.second->Exec(animVar);
		}
		lpTarget->SetWorldPos(animVar.position);
		animVar.elapsedTime += deltaTime;
		animVar.tick = (int)(animVar.elapsedTime / tickScale + FLT_EPSILON);
		if (animVar.tick > *sAnimTick.crbegin())
		{
			isPlay = false;
			//animVar.elapsedTime = 0.0f;
		}
	}
}

void AnimationUIController::Render(HDC hdc)
{

}

void AnimationUIController::Play()
{
	if (lpTarget)
	{
		animVar.elapsedTime = 0.0f;
		animVar.tick = 0;
		animVar.sequence = 0;
		tickIter = sAnimTick.begin();

		isPlay = true;
	}
}

void AnimationUIController::Resume()
{
	if (lpTarget)
	{
		isPlay = true;
	}
}

void AnimationUIController::Stop()
{
	isPlay = false;
}

void AnimationUIController::AddEventTime(float time)
{
	int tick = (int)(time / tickScale + FLT_EPSILON);
	AddEventTick(tick);
}

void AnimationUIController::AddEventTick(int timeTick)
{
	if (-1 < timeTick && timeTick < 10 / tickScale )
	{
		animVar.position = lpTarget->GetWorldPos();
		animVar.tick = timeTick;
		animVar.elapsedTime = timeTick * tickScale;

		auto fit = sAnimTick.find(timeTick);
		if (fit == sAnimTick.end())
		{
			auto it = sAnimTick.insert(timeTick);
			int index = distance(sAnimTick.begin(), it.first);
			for (const auto& pair : mAnimHandler)
			{
				pair.second->AddEvent(index, animVar);
			}
		}
		else
		{
			int index = distance(sAnimTick.begin(), fit);
			for (const auto& pair : mAnimHandler)
			{
				pair.second->ReplaceEvent(index, animVar);
			}
		}
	}
}
