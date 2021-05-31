#include "ParticleManager.h"
#include "ParticleSystem.h"

void ParticleManager::AddParticleSystem(string key, ParticleSystem* lpParticleSystem)
{
	if (lpParticleSystem)
	{
		mParticleSystem.insert(make_pair(key, lpParticleSystem));
	}
}

void ParticleManager::RemoveParticleSystem(string key)
{
	mParticleSystem.erase(key);
}

void ParticleManager::SpreadParticle(string system, POINTFLOAT pos, POINT size, function<void()> callBack)
{
	if (mParticleSystem.find(system) != mParticleSystem.end())
	{
		mParticleSystem[system]->Spread(pos, size.x, size.y, callBack);
	}
}

void ParticleManager::SpreadParticle(string system, POINTFLOAT pos, string data, POINT size, function<void()> callBack)
{
	if (mParticleSystem.find(system) != mParticleSystem.end())
	{
		mParticleSystem[system]->Spread(pos, data, size.x, size.y, callBack);
	}
}
