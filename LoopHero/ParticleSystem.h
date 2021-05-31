#pragma once
#include "GameNode.h"
#include <Windows.h>
#include <list>
#include <regex>
#include <functional>

using namespace std;

enum class PARTICLE_MOVEMENT
{
	STRAIGHT,
	GRAVITY
};

enum class PATTICLE_TYPE
{
	IMAGE,
	TEXT
};

enum class PARTICLE_SPREAD
{
	CONE,
	LINE
};

enum class PARTICLE_ALIGN
{
	CENTER,
	MIDDLE_BOTTOM
};

struct ParticleInfo
{
	PATTICLE_TYPE type;
	PARTICLE_MOVEMENT movement;
	PARTICLE_SPREAD spread;
	PARTICLE_ALIGN align;
	float force;
	float deltaForce;
	float angle;
	float deltaAngle;
	float lifeCycle;
};

class Image;
class Particle;
class ParticleSystem : public GameNode
{
private:
	ParticleInfo info;
	string data;

	list<Particle*> lParticles;

public:
	void Init(ParticleInfo& info, string data = "");
	void Release();
	void Update(float deltaTime);
	void Render(HDC hdc);

	void Spread(POINTFLOAT pos, int width = -1, int height = -1, function<void()> callBack = nullptr);
	void Spread(POINTFLOAT pos, string data, int width = -1, int height = -1, function<void()> callBack = nullptr);
};

