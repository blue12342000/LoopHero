#pragma once
#include "GameUI.h"

class Image;
class GameObject;
class UISprite : public GameUI
{
protected:
	Image* lpImage;
	GameObject* lpObject;

public:
	virtual ~UISprite() {}

	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height);
	virtual void Render(HDC hdc) override;

	virtual bool OnClick(POINT pos) override;

	inline void SetObject(GameObject* lpObject)
	{
		this->lpObject = lpObject;
	}
};

