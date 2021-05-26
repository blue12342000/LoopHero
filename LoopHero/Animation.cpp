#include "Animation.h"
#include "Image.h"

void Animation::Init(Image* lpImage, ANIMATION_TYPE type, float fps)
{
	this->lpImage = lpImage;
	this->type = type;
	this->fps = fps;

	startFrame = 0;
	endFrame = 0;

	state = ANIMATION_STATE::STOP;
}

void Animation::Init(string imageKey, ANIMATION_TYPE type, float fps)
{
	Init(ImageManager::GetSingleton()->FindImage(imageKey), type, fps);
}

void Animation::Update(float deltaTime)
{
	if (state == ANIMATION_STATE::PLAY)
	{
		frame += this->fps * deltaTime;
		if (frame >= endFrame)
		{
			frame = startFrame;
			if (callBack) callBack();
		}
	}
}

void Animation::Render(HDC hdc, int x, int y, IMAGE_ALIGN align)
{
	if (lpImage) lpImage->Render(hdc, x, y, (int)frame, align);
}

void Animation::Play(int startFrame, int endFrame)
{
	if (!lpImage)
	{
		if (callBack) callBack();
		return;
	}

	this->startFrame = startFrame;
	if (endFrame < 0) this->endFrame = lpImage->GetTotalFrame();
	else this->endFrame = endFrame;

	state = ANIMATION_STATE::PLAY;
	frame = startFrame;
}

void Animation::Resume()
{
	state = ANIMATION_STATE::PLAY;
}

void Animation::Stop()
{
	state = ANIMATION_STATE::STOP;
}

void Animation::SetCallBack(function<void()> func)
{
	callBack = move(func);
}

int Animation::GetWidth()
{
	return (lpImage) ? lpImage->GetWidth() : 0;
}

int Animation::GetHeight()
{
	return (lpImage) ? lpImage->GetHeight() : 0;
}