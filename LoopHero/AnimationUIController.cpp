#include "AnimationUIController.h"
#include "AnimationHandler.h"
#include "AnimationMove.h"
#include "PoolingManager.h"
#include "GameUI.h"

void AnimationUIController::Init(GameUI* lpTarget)
{
	this->lpTarget = lpTarget;
	this->isPlay = false;

	vAnimTick.push_back(AnimTick{0, vector<string>()});

	AddAnimationHandler<AnimationMove>();
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
	vAnimTick.clear();
	PoolingManager::GetSingleton()->AddClass(this);
}

void AnimationUIController::Update(float deltaTime)
{
	if (isPlay)
	{
		for (const auto& pair : mAnimHandler)
		{
			pair.second->Exec(animVar);
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
		animVar.position = animVar.origin = lpTarget->GetWorldPos();
		animVar.elapsedTime = 0.0f;
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

void AnimationUIController::AddEvent(float time)
{

}
