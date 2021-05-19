#pragma once
#include "GameUI.h"

class UIGrid : public GameUI
{
private:
	struct Cell
	{
		int index;
		RECT rc;
	};

private:
	int rows;
	int cols;
	POINTFLOAT spacing;

	vector<vector<Cell>> vCells;


public:
	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, int rows, int cols, POINTFLOAT spacing = { 2, 2 });
	void Resize(int rows, int cols);
};

