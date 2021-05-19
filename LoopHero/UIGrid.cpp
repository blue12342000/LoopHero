#include "UIGrid.h"

void UIGrid::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, int rows, int cols, POINTFLOAT spacing)
{
	GameUI::Init(anchor, pos, width, height);
	Resize(rows, cols);
}

void UIGrid::Resize(int rows, int cols)
{
	this->rows = rows;
	this->cols = cols;

	int cWidth = (width - spacing.x * (cols - 1)) / cols;
	int cHeight = (height - spacing.y * (rows - 1)) / rows;

	POINTFLOAT pos;
	vCells.resize(rows);
	for (int y = 0; y < rows; ++y)
	{
		vCells[y].resize(cols);
		for (int x = 0; x < cols; ++x)
		{
			vCells[y][x].rc;

			switch (anchor)
			{
			case UI_ANCHOR::LEFT_TOP:
				SetRect(&vCells[y][x].rc, origin.x, origin.y, origin.x + cWidth, origin.y + cHeight);
				break;
			case UI_ANCHOR::RIGHT_TOP:
				SetRect(&vCells[y][x].rc, origin.x - cWidth, origin.y, origin.x, origin.y + cHeight);
				break;
			case UI_ANCHOR::LEFT_BOTTOM:
				SetRect(&vCells[y][x].rc, origin.x, origin.y - cHeight, origin.x + cWidth, origin.y);
				break;
			case UI_ANCHOR::RIGHT_BOTTOM:
				SetRect(&vCells[y][x].rc, origin.x - cWidth, origin.y - cHeight, origin.x, origin.y);
				break;
			case UI_ANCHOR::LEFT_MIDDLE:
				SetRect(&vCells[y][x].rc, origin.x, origin.y - cHeight * rows / 2, origin.x + cWidth, origin.y - cHeight * rows / 2 + cHeight);
				break;
			case UI_ANCHOR::RIGHT_MIDDLE:
				break;
			case UI_ANCHOR::TOP_MIDDLE:
				break;
			case UI_ANCHOR::BOTTOM_MIDDLE:
				break;
			case UI_ANCHOR::MIDDLE:
				break;
			}
		}
	}
}
