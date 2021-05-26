#pragma once
#include "GameObject.h"

enum class ANIMATION_STATE
{
	PLAY,
	STOP
};

enum class ANIMATION_TYPE
{
	LOOP,
	ONCE
};

enum class IMAGE_ALIGN;
class Image;
class Animation
{
private:
	ANIMATION_STATE state;
	ANIMATION_TYPE type;
	Image* lpImage;
	float frame;
	float fps;

	int startFrame;
	int endFrame;

	function<void()> callBack;

public:
	void Init(Image* lpImage, ANIMATION_TYPE type, float fps);
	void Init(string imageKey, ANIMATION_TYPE type, float fps);

	void Update(float deltaTime);
	void Render(HDC hdc, int x, int y, IMAGE_ALIGN align = (IMAGE_ALIGN)0);

	void Play(int startFrame = 0, int endFrame = -1);
	void Resume();
	void Stop();

	void SetCallBack(function<void()> func);

	int GetWidth();
	int GetHeight();
};

