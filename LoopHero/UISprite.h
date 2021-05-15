#pragma once
#include "GameUI.h"

class Image;
class GameObject;
class UISprite : public GameUI
{
private:
	Image* lpImage;
	GameObject* lpObject;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height);
	virtual void Render(HDC hdc) override;

	inline void SetObject(GameObject* lpObject)
	{
		this->lpObject = lpObject;
	}
};

