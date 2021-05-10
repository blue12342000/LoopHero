#pragma once
#include <Windows.h>

class Timer
{
private:
	bool hasGoodHardWare;
	float timeScale;		//��� ������/�ʴ� ������ -> �׷��� ���������� ����� ������ �ð��� ��������?
	float deltaTime;
	__int64 currTime;
	__int64 lastTime;
	__int64 periodFrequency;

	float fpsTimeElapsed;			// fps
	unsigned long fpsFrameCount;	// FPS ���°�
	unsigned long FPS;				// �ʴ� ������ �� frame per second?

	char lpszText[128];

public:
	HRESULT Init(); 
	void Tick();
	void Render(HDC hdc);

	inline unsigned long GetFPS() { return this->FPS; }
	inline float GetDeltaTime() { return this->deltaTime; }

};

