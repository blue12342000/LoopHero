#pragma once
#include "Scene.h"

class Image;
class GameUI;
class EventSystem;
class Title : public Scene
{
private:
	Image* lpBuffer;
	Image* lpBackImage;

	GameUI* lpCanvus;
	EventSystem* lpEventSystem;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
};

