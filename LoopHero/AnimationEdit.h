#pragma once
#include "Scene.h"

enum class EDIT_SCENE
{
	TITLE,
	INGAME,
	NONE
};

class Image;
class GameUI;
class EventSystem;
class UIButton;
class UIDebug;
class UIHierarchy;
class AnimationEdit : public Scene
{
private:
	Image* lpBuffer;
	Image* lpBackImage;

	UIButton* lpButton[(int)EDIT_SCENE::NONE];

	GameUI* lpCanvus;

	UIDebug* lpUIDebug;
	UIHierarchy* lpHierarchy;

	EventSystem* lpEventSystem;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	void LoadTitleUI();
	void LoadInGameUI();
};

