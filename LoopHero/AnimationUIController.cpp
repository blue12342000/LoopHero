#include "AnimationUIController.h"
#include "AnimationHandler.h"
#include "AnimationMove.h"
#include "PoolingManager.h"
#include "GameUI.h"

void AnimationUIController::Init(GameUI* lpTarget)
{
	vAnimVars.reserve(1000);
	this->lpTarget = lpTarget;
	this->isPlay = false;
	this->tickScale = 0.01f;

	animVar.isLinear = false;
	animVar.sequence = 0;
	AddAnimationHandler<AnimationMove>();
	AddEventTick(0);
}

void AnimationUIController::Release()
{
	lpTarget = nullptr;
	isPlay = false;
	animVar.isLinear = false;
	animVar.sequence = 0;
	for (const auto& pair : mAnimHandler)
	{
		pair.second->Release();
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

void AnimationUIController::ResetEvent()
{
	if (sAnimTick.size() > 1)
	{
		for (const auto& pair : mAnimHandler)
		{
			pair.second->ResetEvent();
		}
		sAnimTick.clear();
		sAnimTick.insert(0);
		tickIter = sAnimTick.begin();
	}
}

void AnimationUIController::AddEventTime(float time)
{
	int tick = (int)(time / tickScale + FLT_EPSILON);
	AddEventTick(tick);
}

void AnimationUIController::AddEventTick(int timeTick)
{
	if (-1 < timeTick && timeTick <= 10 / tickScale )
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
		tickIter = sAnimTick.begin();
	}
}

vector<AnimVariable> AnimationUIController::GetAnimVariables()
{
	vAnimVars.clear();
	AnimVariable animVariable;
	animVariable.isLinear = animVar.isLinear;
	animVariable.origin = animVar.origin;
	animVariable.position = animVar.origin;
	animVariable.elapsedTime = 0.0f;
	animVariable.tick = 0;
	animVariable.sequence = 0;
	if (!sAnimTick.empty())
	{
		set<int>::iterator sit = sAnimTick.begin();
		while (animVariable.tick <= *sAnimTick.crbegin())
		{
			if (*sit < animVariable.tick)
			{
				++sit;
				++animVariable.sequence;
			}

			for (const auto& pair : mAnimHandler)
			{
				pair.second->Exec(animVariable);
			}
			vAnimVars.push_back(animVariable);

			animVariable.elapsedTime += tickScale;
			animVariable.tick = (int)(animVariable.elapsedTime / tickScale + FLT_EPSILON);
		}
	}

	return vAnimVars;
}