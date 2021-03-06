#pragma once
#include "GameUI.h"

class Text;
class UIButton;
class UIHierarchyItem : public GameUI
{
private:
	Text* lpText;
	GameUI* lpTarget;
	bool isHover;

	bool isViewButton;
	UIButton* lpViewToggle;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Release() override;
	virtual void Render(HDC hdc) override;

	void SetTarget(GameUI* lpTarget, int depth);
	void SetViewButton(bool isViewButton);

	void OnClick(EventData& data) override;
	void OnMouseEnter(EventData& data) override;
	void OnMouseOver(EventData& data) override;
	void OnMouseOut(EventData& data) override;

	inline GameUI* GetTarget() { return lpTarget; }
};

