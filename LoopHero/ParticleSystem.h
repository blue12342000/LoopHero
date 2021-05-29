#pragma once
#include "GameNode.h"
#include <vector>

using namespace std;

enum class PARTICLE_MOVEMENT
{
	STRAIGHT,
	GRAVITY
};

class Particle;
class ParticleSystem : public GameNode
{
private:
	vector<Particle*> vParticles;

public:
};

