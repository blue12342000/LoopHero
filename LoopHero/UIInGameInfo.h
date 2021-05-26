#pragma once
#include "GameUI.h"

class UITextField;
class UIInGameInfo : public GameUI
{
private:
	UITextField* lpTitle;
	UITextField* lpContent;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	//virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;
	
	void SetTitle(string text);
	void SetContent(string text);

	void ViewInfo(ObserverHandler* lpTarget);
	void HideInfo(ObserverHandler* lpTarget);
};

