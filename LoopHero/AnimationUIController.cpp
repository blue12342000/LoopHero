#include "AnimationUIController.h"
#include "AnimationHandler.h"
#include "AnimationMove.h"
#include "PoolingManager.h"
#include "DataManager.h"
#include "GameUI.h"
#include "Utill.h"
#include <regex>

void AnimationUIController::AddEventTick(int timeTick, POINTFLOAT pos)
{
	if (-1 < timeTick && timeTick <= 10 / tickScale)
	{
		animVar.position = pos;
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

void AnimationUIController::Init(GameUI* lpTarget, string anim)
{
	vAnimVars.reserve(1000);
	this->state = ANIMAION_STATE::READY;
	this->lpTarget = lpTarget;
	this->tickScale = 0.01f;

	animVar.isLinear = false;
	animVar.sequence = 0;
	AddAnimationHandler<AnimationMove>();
	// 데이터가 없을경우를 대비한 초기데이터

	animKey = anim;
	AddEventTick(0);
	Load();
}

void AnimationUIController::Release()
{
	lpTarget = nullptr;
	state = ANIMAION_STATE::READY;
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
	if (state == ANIMAION_STATE::READY)
	{
		animVar.elapsedTime = 0.0f;
		animVar.tick = 0;
		animVar.sequence = 0;
		tickIter = sAnimTick.begin();

		state = ANIMAION_STATE::PLAY;
	}
	else if (state == ANIMAION_STATE::PLAY)
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
		if (animVar.tick > *sAnimTick.crbegin())
		{
			state = ANIMAION_STATE::STOP;
		}

		animVar.elapsedTime += deltaTime;
		animVar.tick = (int)(animVar.elapsedTime / tickScale + FLT_EPSILON);
	}
}

void AnimationUIController::Render(HDC hdc)
{

}

void AnimationUIController::Load()
{
	map<string, string> mAnimDatas = DataManager::GetSingleton()->GetData("animations", animKey);
	if (!mAnimDatas.empty())
	{
		regex reg("^tick_event_[0-9]{1,}$");
		vector<string> vTickEvent;
		for (const auto& pair : mAnimDatas)
		{
			if (regex_match(pair.first, reg))
			{
				vTickEvent = move(StringSplit(pair.second, '|'));
				if (vTickEvent.size() != 3) continue;

				AddEventTick(stoi(vTickEvent[0]), POINTFLOAT{ stof(vTickEvent[1]), stof(vTickEvent[2]) });
			}
		}

		if (mAnimDatas.find("anim_linear") != mAnimDatas.end())
		{
			animVar.isLinear = (mAnimDatas["anim_linear"] == "1");
		}
		if (mAnimDatas.find("target_anchor") != mAnimDatas.end())
		{
			lpTarget->SetAnchor((UI_ANCHOR)stoi(mAnimDatas["target_anchor"]));
		}
	}
}

void AnimationUIController::Save()
{
	map<string, string> mData;

	AnimVariable animVariable;
	animVariable.isLinear = animVar.isLinear;
	animVariable.origin = animVar.origin;
	animVariable.position = animVar.origin;
	animVariable.elapsedTime = 0.0f;
	animVariable.tick = 0;
	animVariable.sequence = 0;

	string key, value;
	set<int>::iterator sit = sAnimTick.cbegin();
	while (sit != sAnimTick.cend())
	{
		animVariable.tick = *sit;

		for (const auto& pair : mAnimHandler)
		{
			pair.second->Exec(animVariable);
		}

		key = "tick_event_" + to_string(animVariable.sequence);
		value = to_string(animVariable.tick) + "|" + to_string(animVariable.position.x) + "|" + to_string(animVariable.position.y);
		mData.insert(make_pair(move(key), move(value)));

		++sit;
		++animVariable.sequence;
		animVariable.elapsedTime += tickScale;
	}
	mData.insert(make_pair("anim_linear", to_string(animVar.isLinear)));
	mData.insert(make_pair("target_anchor", to_string((int)lpTarget->GetAnchor())));

	DataManager::GetSingleton()->ReplaceData("animations", animKey, mData);
	DataManager::GetSingleton()->SaveIniFile("Ini/animation.ini", "animations");
}

void AnimationUIController::Reset()
{

}

void AnimationUIController::Play()
{
	if (lpTarget)
	{
		state = ANIMAION_STATE::READY;
	}
}

void AnimationUIController::Resume()
{
	if (lpTarget)
	{
		state = ANIMAION_STATE::PLAY;
	}
}

void AnimationUIController::Stop()
{
	state = ANIMAION_STATE::STOP;
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