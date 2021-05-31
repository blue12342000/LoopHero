#include "ParticleSystem.h"
#include "PoolingManager.h"
#include "Particle.h"
#include "GameObject.h"
#include "GameUI.h"
#include "Text.h"
#include "ImageManager.h"
#include "Animation.h"
#include "Image.h"
#include "Utill.h"

void ParticleSystem::Init(ParticleInfo& info, string data)
{
	this->info = info;
	this->data = data;
}

void ParticleSystem::Release()
{
	for (auto cit = lParticles.cbegin(), end = lParticles.cend(); cit != end; ++cit)
	{
		(*cit)->Release();
	}
	lParticles.clear();
	data = "";
	PoolingManager::GetSingleton()->AddClass(this);
}

void ParticleSystem::Update(float deltaTime)
{
	for (auto cit = lParticles.cbegin(), end = lParticles.cend(); cit != end; ++cit)
	{
		if ((*cit)->IsDective())
		{
			(*cit)->Release();
			cit = lParticles.erase(cit);
			end = lParticles.cend();
			if (cit == end) break;
		}

		(*cit)->Update(deltaTime);
	}
}

void ParticleSystem::Render(HDC hdc)
{
	for (auto cit = lParticles.cbegin(), end = lParticles.cend(); cit != end; ++cit)
	{
		(*cit)->Render(hdc);
	}
}

void ParticleSystem::Spread(POINTFLOAT pos, int width, int height, function<void()> callBack)
{
	Particle* lpParticle = nullptr;
	Transform transform;
	transform.pos = pos;
	transform.width = width;
	transform.height = height;

	if (info.force > FLT_EPSILON)
	{
		float targetForce = (rand() % (int)(info.deltaForce * 100 - info.deltaForce * 100 * 0.5f)) * 0.01f + info.force;
		if (info.spread == PARTICLE_SPREAD::CONE)
		{
			float targetAngle = (rand() % (int)(info.deltaAngle * 100) - info.deltaAngle * 100 * 0.5f) * 0.01f + info.angle;
			transform.velocity.x = cos(targetAngle) * targetForce;
			transform.velocity.y = sin(targetAngle) * targetForce;
		}
		else
		{
			transform.velocity.x = cos(info.angle) * targetForce;
			transform.velocity.y = sin(info.angle) * targetForce;
		}
	}
	else
	{
		transform.velocity.x = 0.0f;
		transform.velocity.y = 0.0f;
	}

	if (info.type == PATTICLE_TYPE::IMAGE)
	{
		Image* lpImage = ImageManager::GetSingleton()->FindImage(data);
		if (!lpImage) return;

		lpParticle = PoolingManager::GetSingleton()->GetClass<Particle>();

		Animation* lpAnimation = PoolingManager::GetSingleton()->GetClass<Animation>();
		lpAnimation->Init(lpImage, ANIMATION_TYPE::ONCE, lpImage->GetTotalFrame() / info.lifeCycle);
		lpParticle->Init(info, transform, lpAnimation);
	}
	else if (info.type == PATTICLE_TYPE::TEXT)
	{
		lpParticle = PoolingManager::GetSingleton()->GetClass<Particle>();

		Text* lpText = PoolingManager::GetSingleton()->GetClass<Text>();
		lpText->SetText(data);

		lpParticle->Init(info, transform, lpText);
	}

	if (lpParticle)
	{
		if (callBack) lpParticle->SetCallBack(callBack);
		lParticles.push_back(lpParticle);
	}
}

void ParticleSystem::Spread(POINTFLOAT pos, string data, int width, int height, function<void()> callBack)
{
	Particle* lpParticle = nullptr;
	Transform transform;
	transform.pos = pos;
	transform.width = width;
	transform.height = height;

	float targetForce = (rand() % (int)(info.deltaForce * 100 - info.deltaForce * 100 * 0.5f)) * 0.01f + info.force;
	if (info.spread == PARTICLE_SPREAD::CONE)
	{
		float targetAngle = (rand() % (int)(info.deltaAngle * 100) - info.deltaAngle * 100 * 0.5f) * 0.01f + info.angle;
		transform.velocity.x = cos(targetAngle) * targetForce;
		transform.velocity.y = sin(targetAngle) * targetForce;
	}
	else
	{
		transform.velocity.x = cos(info.angle) * targetForce;
		transform.velocity.y = sin(info.angle) * targetForce;
	}

	if (info.type == PATTICLE_TYPE::IMAGE)
	{
		Image* lpImage = ImageManager::GetSingleton()->FindImage(data);
		if (!lpImage) return;

		lpParticle = PoolingManager::GetSingleton()->GetClass<Particle>();

		Animation* lpAnimation = PoolingManager::GetSingleton()->GetClass<Animation>();
		lpAnimation->Init(lpImage, ANIMATION_TYPE::ONCE, lpImage->GetTotalFrame() / info.lifeCycle);

		lpParticle->Init(info, transform, lpAnimation);
	}
	else if (info.type == PATTICLE_TYPE::TEXT)
	{
		lpParticle = PoolingManager::GetSingleton()->GetClass<Particle>();

		vector<string> vText = StringSplit(data, '|');
		regex reg("#[0-9A-F]{6}");
		long color;
		if (regex_match(vText[0], reg))
		{
			color = strtol(vText[0].substr(1).c_str(), nullptr, 16);
			data = data.substr(8);
		}
		else
		{
			color = RGB(255, 255, 255);
		}

		Text* lpText = PoolingManager::GetSingleton()->GetClass<Text>();
		lpText->Init("Bµ¸¿ò", 10, color);
		lpText->SetText(data);

		lpParticle->Init(info, transform, lpText);
	}

	if (lpParticle)
	{
		if (callBack) lpParticle->SetCallBack(callBack);
		lParticles.push_back(lpParticle);
	}
}