#include "Animation.h"
#include "Image.h"

void Animation::Init(Image* lpImage, ANIMATION_TYPE type, float fps)
{
	this->lpImage = lpImage;
	this->type = type;
	this->fps = fps;

	if (lpImage)
	{
		this->width = lpImage->GetWidth();
		this->height = lpImage->GetHeight();
	}
	else
	{
		this->width = -1;
		this->height = -1;
	}

	startFrame = 0;
	endFrame = 0;

	state = ANIMATION_STATE::STOP;
}

void Animation::Init(string imageKey, ANIMATION_TYPE type, float fps)
{
	Init(ImageManager::GetSingleton()->FindImage(imageKey), type, fps);
}

void Animation::Init(Image* lpImage, ANIMATION_TYPE type, int width, int height, float fps)
{
	this->lpImage = lpImage;
	this->type = type;
	this->fps = fps;
	this->width = width;
	this->height = height;

	startFrame = 0;
	endFrame = 0;

	state = ANIMATION_STATE::STOP;
}

void Animation::Release()
{
	lpImage = nullptr;
	callBack = nullptr;
	state = ANIMATION_STATE::STOP;
	PoolingManager::GetSingleton()->AddClass(this);
}

void Animation::Update(float deltaTime)
{
	if (state == ANIMATION_STATE::PLAY)
	{
		frame += fps * deltaTime;
		if (frame >= endFrame)
		{
			if (type == ANIMATION_TYPE::LOOP)
			{
				frame = startFrame;
			}
			else
			{
				state = ANIMATION_STATE::STOP;
				frame = endFrame - 1;
			}
			if (callBack) callBack();
		}
	}
}

void Animation::Render(HDC hdc, int x, int y, IMAGE_ALIGN align)
{
	if (lpImage)
	{
		if (lpImage->IsFrameLoop())
		{
			lpImage->LoopRender(hdc, POINT{ x, y }, width, height, (int)frame, align);
		}
		else
		{
			lpImage->Render(hdc, x, y, (int)frame, align);
		}
	}
}

void Animation::Render(HDC hdc, int x, int y, int width, int height, IMAGE_ALIGN align)
{
	if (lpImage)
	{
		Ellipse(hdc, x - 5, y - 5, x + 5, y + 5);

		if (lpImage->IsFrameLoop())
		{
			lpImage->LoopRender(hdc, POINT{ x, y }, width, height, (int)frame, align);
		}
		else
		{
			lpImage->Render(hdc, x, y, (int)frame, align);
		}
	}
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