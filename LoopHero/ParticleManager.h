#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <string>
#include <map>
#include <functional>

using namespace std;

class ParticleSystem;
class ParticleManager : public Singleton<ParticleManager>
{
private:
	map<string, ParticleSystem*> mParticleSystem;

public:
	void AddParticleSystem(string key, ParticleSystem* lpParticleSystem);
	void RemoveParticleSystem(string key);

	void SpreadParticle(string system, POINTFLOAT pos, POINT size = {-1, -1}, function<void()> callBack = nullptr);
	void SpreadParticle(string system, POINTFLOAT pos, string data, POINT size = {-1, -1}, function<void()> callBack = nullptr);
};

