#include "Timer.h"

HRESULT Timer::Init()
{
	hasGoodHardWare = false;
	timeScale = 0.0f;		// 경과 진동수/초당 진동수 -> 한 루프가 도는 동안의 시간
	deltaTime = 0.0f;		// 경과 시간 : 현재시간 - 이전에 체크한 시간(last time)
	currTime = 0.0;			// 현재 시간
	lastTime = 0.0;			// 마지막에 체크한 시간
	periodFrequency = 0.0;	// 1초당 진동수 저장할 변수

	fpsTimeElapsed = 0.0f;
	fpsFrameCount = 0;
	
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&periodFrequency))	//고해상도 1초당 진동수(돈 루프 수) 반환
	{
		hasGoodHardWare = true;

		//수행시간동안 발생한 진동수 반환, 함수가 호출된 시점(여기선 라스트 타임)에 고해상도 타이머 값 설정
		QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);
		//periodFrequency - 1초에 몇루프 도는가(진동수)이므로 timeScale -한 루프 도는데 걸리는 시간은
		// 1s : periodFrequency루프 = timeScale : 1루프 -> 비례식 계산
		timeScale = 1.0f / periodFrequency;
	}
	else
	{
		hasGoodHardWare = false;
		lastTime = timeGetTime();
		timeScale = 0.001f;		//대략적인 계산 - 원래하던 SetTimer와 같다.
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
		// timeGetTime() 윈도우 시작되고 지금까지 흐른 시간을 반환하는 함수
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

void Timer::Render(HDC hdc)
{
	//확인용 출력
	wsprintf(lpszText, "FPS : %d", FPS);
	TextOut(hdc, 20, 20, lpszText, strlen(lpszText));
}
