#include "Particle.h"
#include "Animation.h"
#include "Image.h"
#include "Text.h"

void Particle::Init(const ParticleInfo& info, Transform transform, Animation* lpGameNode)
{
	this->isActive = true;
	this->transform = move(transform);
	this->elapsedTime = 0.0f;
	this->info = info;
	this->lpAnimation = lpGameNode;
	this->callBack = nullptr;
	lpAnimation->Play();
}

void Particle::Init(const ParticleInfo& info, Transform transform, Text* lpGameNode)
{
	this->isActive = true;
	this->transform = move(transform);
	this->elapsedTime = 0.0f;
	this->info = info;
	this->lpText = lpGameNode;
	this->callBack = nullptr;
}

void Particle::Release()
{
	if (lpAnimation)
	{
		lpAnimation->Release();
		lpAnimation = nullptr;
	}

	if (lpText)
	{
		lpText->Release();
		lpText = nullptr;
	}

	callBack = nullptr;

	PoolingManager::GetSingleton()->AddClass(this);
}

void Particle::Update(float deltaTime)
{
	if (!isActive) return;

	transform.pos.x += transform.velocity.x * deltaTime;
	transform.pos.y += transform.velocity.y * deltaTime;
	if (info.movement == PARTICLE_MOVEMENT::GRAVITY)
	{
		transform.velocity.y += 9.80665f * GRAVITY_SCALA * deltaTime;
	}

	if (info.type == PATTICLE_TYPE::IMAGE)
	{
		if (lpAnimation) lpAnimation->Update(deltaTime);
	}

	elapsedTime += deltaTime;
	isActive = elapsedTime < info.lifeCycle;
	if (isActive == false && callBack) callBack();
}

void Particle::Render(HDC hdc)
{
	if (!isActive) return;

	if (info.type == PATTICLE_TYPE::IMAGE)
	{
		if (lpAnimation)
		{
			if (transform.width < 0 || transform.height < 0)
			{
				switch (info.align)
				{
				case PARTICLE_ALIGN::CENTER:
					lpAnimation->Render(hdc, (int)transform.pos.x, (int)transform.pos.y, IMAGE_ALIGN::CENTER);
					break;
				case PARTICLE_ALIGN::MIDDLE_BOTTOM:
					lpAnimation->Render(hdc, (int)transform.pos.x, (int)transform.pos.y, IMAGE_ALIGN::MIDDLE_BOTTOM);
					break;
				}
			}
			else
			{
				switch (info.align)
				{
				case PARTICLE_ALIGN::CENTER:
					lpAnimation->Render(hdc, (int)transform.pos.x, (int)transform.pos.y, transform.width, transform.height, IMAGE_ALIGN::CENTER);
					break;
				case PARTICLE_ALIGN::MIDDLE_BOTTOM:
					lpAnimation->Render(hdc, (int)transform.pos.x, (int)transform.pos.y, transform.width, transform.height, IMAGE_ALIGN::MIDDLE_BOTTOM);
					break;
				}
			}
		}
	}
	else if (info.type == PATTICLE_TYPE::TEXT)
	{
		if (lpText)
		{
			if (transform.width < 0 || transform.height < 0)
			{
				switch (info.align)
				{
				case PARTICLE_ALIGN::CENTER:
					lpText->Render(hdc, (int)transform.pos.x, (int)transform.pos.y, TA_CENTER);
					break;
				case PARTICLE_ALIGN::MIDDLE_BOTTOM:
					lpText->Render(hdc, (int)transform.pos.x, (int)transform.pos.y, TA_CENTER | TA_BOTTOM);
					break;
				}
			}
			else
			{
				RECT inRect;
				inRect.left = (int)transform.pos.x - transform.width / 2;
				inRect.right = inRect.left + transform.width;
				inRect.top = (int)transform.pos.y - transform.height / 2;
				inRect.bottom = inRect.top + transform.height;

				switch (info.align)
				{
				case PARTICLE_ALIGN::CENTER:
					lpText->Render(hdc, inRect, TA_CENTER);
					break;
				case PARTICLE_ALIGN::MIDDLE_BOTTOM:
					lpText->Render(hdc, inRect, TA_CENTER | TA_BOTTOM);
					break;
				}
			}
		}
	}
}
