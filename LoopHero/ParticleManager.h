#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <list>

using namespace std;

class Particle;
class ParticleManager : public Singleton<ParticleManager>
{
private:
	list<Particle*> lParticles;

public:
	void Update(float deltaTime);
	void Render(HDC hdc);

	void AddParticle(Particle* lpParticle);
};

