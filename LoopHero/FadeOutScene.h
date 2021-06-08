#pragma once
#include "LoadingScene.h"
#include <vector>
#include <algorithm>

#define VIEW_SPLIT_X 80
#define VIEW_SPLIT_Y 50

using namespace std;

class Image;
class FadeOutScene : public LoadingScene
{
private:
	enum class FADE_IN_OUT
	{
		FADE_OUT,
		FADE_IN
	};

private:
	FADE_IN_OUT inOut;
	Image* lpBuffer;
	Image* lpInBuffer;
	vector<int> splitView;
	float elapsedTime;
	int width;
	int height;
	int lastProcess;
	float fadeSpeed;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

};

