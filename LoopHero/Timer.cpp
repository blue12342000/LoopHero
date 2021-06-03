#include "Timer.h"

float Timer::deltaTime = 0.0f;
unsigned long Timer::FPS = 0;

HRESULT Timer::Init()
{
	hasGoodHardWare = false;
	timeScale = 0.0f;
	deltaTime = 0.0f;
	currTime = 0.0;
	lastTime = 0.0;
	periodFrequency = 0.0;

	fpsTimeElapsed = 0.0f;
	fpsFrameCount = 0;
	
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&periodFrequency))
	{
		hasGoodHardWare = true;

		QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);
		timeScale = 1.0f / periodFrequency;
	}
	else
	{
		hasGoodHardWare = false;
		lastTime = timeGetTime();
		timeScale = 0.001f;
	}
    return S_OK;
}

void Timer::Tick()
{
	if (hasGoodHardWare)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	}
	else
	{
		currTime = timeGetTime();
	}

	deltaTime = (currTime - lastTime) * timeScale;

	fpsTimeElapsed += deltaTime;
	fpsFrameCount++;

	if (fpsTimeElapsed >= 1.0f)
	{
		FPS = fpsFrameCount;
		fpsFrameCount = 0.0f;
		fpsTimeElapsed -= 1.0f;
	}
	lastTime = currTime;
}