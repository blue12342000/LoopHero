#pragma once
#include "GameUI.h"

class UIGrid : public GameUI
{
private:
	struct UICell
	{
		GameUI* lpGameUI;
		POINTFLOAT pos;
	};

private:
	int rows;
	int cols;
	POINTFLOAT spacing;

	vector<vector<UICell>> vCells;


public:
	virtual ~UIGrid() {}

	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, int rows, int cols, POINTFLOAT spacing = { 0, 0 });
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc);

	void Resize(int rows, int cols);

	void SetItemObject(int row, int col, GameUI* lpGameUI);

	virtual void AddChildUI(GameUI* lpChild) final;
	virtual void RemoveChildUI(int index = 0) final;

	virtual void OnDrop(EventData& data) override;
};

