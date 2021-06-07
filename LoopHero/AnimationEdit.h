#pragma once
#include "Scene.h"

class Image;
class GameUI;
class EventSystem;
class UIButton;
class UIDebug;
class UIHierarchy;
class UIAnimInspector;
class AnimationEdit : public Scene
{
private:
	Image* lpBuffer;
	Image* lpBackImage;


	GameUI* lpCanvus;

	UIDebug* lpUIDebug;
	UIHierarchy* lpHierarchy;
	UIAnimInspector* lpUIAnimInspector;

	EventSystem* lpEventSystem;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	void ResetInit();
	void LoadTitleUI();
	void LoadInGameUI();
};

