#pragma once
#include <Windows.h>

class Timer
{
private:
	bool hasGoodHardWare;
	float timeScale;
	__int64 currTime;
	__int64 lastTime;
	__int64 periodFrequency;

	float fpsTimeElapsed;
	unsigned long fpsFrameCount;

public:
	static float deltaTime;			
	static unsigned long FPS;

public:
	HRESULT Init(); 
	void Tick();
};

