#pragma once
#include "GameUI.h"

class Image;
class UITextField;
class UIDebug : public GameUI
{
private:
	Image* lpBackground;
	UITextField* lpLogText;

	GameNode* lpDebugNode;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	void OpenDebugInfo(ObserverHandler* lpTarget);

	void OnClick(EventData& data) override;
};

