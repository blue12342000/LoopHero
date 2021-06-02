#pragma once
#include "GameUI.h"

class Image;
class UITimeTick : public GameUI
{
private:
	Image* lpBackground;

	bool isUseTick;
	int timeTick;

public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height) override;
	virtual void Render(HDC hdc) override;

	//void OnClick(EventData& data) override;

	inline void SetUseTick(bool isUseTick) { this->isUseTick = isUseTick; }
	inline void SetTick(int tick) { this->timeTick = tick; }
	inline int GetTick() { return timeTick; }
};

