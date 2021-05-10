#pragma once
#include <Windows.h>

class Timer
{
private:
	bool hasGoodHardWare;
	float timeScale;		//경과 진동수/초당 진동수 -> 그러면 한프레임이 경과한 동안의 시간이 구해진다?
	float deltaTime;
	__int64 currTime;
	__int64 lastTime;
	__int64 periodFrequency;

	float fpsTimeElapsed;			// fps
	unsigned long fpsFrameCount;	// FPS 세는거
	unsigned long FPS;				// 초당 프레임 수 frame per second?

	char lpszText[128];

public:
	HRESULT Init(); 
	void Tick();
	void Render(HDC hdc);

	inline unsigned long GetFPS() { return this->FPS; }
	inline float GetDeltaTime() { return this->deltaTime; }

};

