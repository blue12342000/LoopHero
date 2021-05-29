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

	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Release() override;
	virtual void Render(HDC hdc) override;

	inline void SetGameObject(GameObject* lpObject) { this->lpObject = lpObject; }
	inline GameObject* GetGameObject() { return lpObject; }

	void OnMouseOver(EventData& data);
	void OnMouseOut(EventData& data);
};

