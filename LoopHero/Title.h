#pragma once
#include "Scene.h"

class Image;
class Title : public Scene
{
private:
	Image* lpBuffer;
	Image* lpBackImage;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
};

