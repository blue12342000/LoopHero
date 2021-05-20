#include "UIGrid.h"
#include "Utill.h"

void UIGrid::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, int rows, int cols, POINTFLOAT spacing)
{
	GameUI::Init(anchor, pos, width, height);
	Resize(rows, cols);
}

void UIGrid::Update(float deltaTime)
{
	POINTFLOAT currPos, newPos = { 0.0f, 0.0f };
	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			if (vCells[y][x].lpGameUI == nullptr) continue;

			currPos = vCells[y][x].lpGameUI->GetPos();
			newPos = currPos;
			newPos.x += (vCells[y][x].pos.x - currPos.x) * 10 * deltaTime;
			newPos.y += (vCells[y][x].pos.y - currPos.y) * 10 * deltaTime;

			vCells[y][x].lpGameUI->SetPos(newPos);
		}
	}

	GameUI::Update(deltaTime);
}

void UIGrid::Render(HDC hdc)
{
	RenderRectangle(hdc, rc);
	int cWidth = (width - spacing.x * (cols - 1)) / cols;
	int cHeight = (height - spacing.y * (rows - 1)) / rows;
	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			RenderRectangle(hdc, MakeRect(vCells[y][x].pos, cWidth, cHeight), RGB(200, 0, 0));
		}
	}
	GameUI::Render(hdc);
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
			switch (anchor)
			{
			case UI_ANCHOR::RIGHT_TOP:
				vCells[y][x].pos.x = (width - cWidth) - (cWidth + spacing.x) * x;
				vCells[y][x].pos.y = (cHeight + spacing.y) * y;
				break;
			case UI_ANCHOR::LEFT_BOTTOM:
				vCells[y][x].pos.x = (cWidth + spacing.x) * x;
				vCells[y][x].pos.y = (height - cHeight) - (cHeight + spacing.y) * y;
				break;
			case UI_ANCHOR::RIGHT_BOTTOM:
				vCells[y][x].pos.x = (width - cWidth) - (cWidth + spacing.x) * x;
				vCells[y][x].pos.y = (height - cHeight) - (cHeight + spacing.y) * y;
				break;
			case UI_ANCHOR::LEFT_MIDDLE:
				vCells[y][x].pos.x = (cWidth + spacing.x) * x;
				vCells[y][x].pos.y = (cHeight + spacing.y) * y - height / 2;
				break;
			case UI_ANCHOR::RIGHT_MIDDLE:
				vCells[y][x].pos.x = (width - cWidth) - (cWidth + spacing.x) * x;
				vCells[y][x].pos.y = (cHeight + spacing.y) * y - height / 2;
				break;
			case UI_ANCHOR::TOP_MIDDLE:
				vCells[y][x].pos.x = (cWidth + spacing.x) * x - width / 2;
				vCells[y][x].pos.y = (cHeight + spacing.y) * y;
				break;
			case UI_ANCHOR::BOTTOM_MIDDLE:
				vCells[y][x].pos.x = (cWidth + spacing.x) * x - width / 2;
				vCells[y][x].pos.y = (height - cHeight) - (cHeight + spacing.y) * y;
				break;
			case UI_ANCHOR::MIDDLE:
				vCells[y][x].pos.x = (cWidth + spacing.x) * x - width / 2;
				vCells[y][x].pos.y = (cHeight + spacing.y) * y - height / 2;
				break;
			case UI_ANCHOR::LEFT_TOP:
			default:
				vCells[y][x].pos.x = (cWidth + spacing.x) * x;
				vCells[y][x].pos.y = (cHeight + spacing.y) * y;
				break;
			}
		}
	}
}

void UIGrid::SetItemObject(int row, int col, GameUI* lpGameUI)
{
	if (row < 0 || col < 0 || row >= rows || col >= cols) return;
	vCells[row][col].lpGameUI = lpGameUI;
	lpGameUI->SetAnchor(UI_ANCHOR::LEFT_TOP);
	lpGameUI->SetPos(lpGameUI->GetRealationPos(this));
	GameUI::AddChildUI(lpGameUI);
}

void UIGrid::AddChildUI(GameUI* lpChild)
{
	if (!lpChild) return;

	POINT emptySlot = { 0, 0 };
	for (int y = rows - 1; y > -1; --y)
	{
		for (int x = cols - 1; x > -1; --x)
		{
			if (vCells[y][x].lpGameUI == nullptr)
			{
				emptySlot.x = x;
				emptySlot.y = y;
			}
			else if (vCells[y][x].lpGameUI == lpChild)
			{
				return;
			}
		}
	}
	lpChild->SetAnchor(UI_ANCHOR::LEFT_TOP);
	lpChild->SetPos(lpChild->GetRealationPos(this));

	GameUI::AddChildUI(lpChild);
}

void UIGrid::RemoveChildUI(int index)
{
	GameUI* lpItem = *(vChildUI.begin() + index);

	for (int y = rows - 1; y > -1; --y)
	{
		for (int x = cols - 1; x > -1; --x)
		{
			if (vCells[y][x].lpGameUI == lpItem)
			{
				vCells[y][x].lpGameUI = nullptr;
				GameUI::RemoveChildUI(index);
				return;
			}
		}
	}
}

void UIGrid::OnDrop(EventData& data)
{
	MessageBox(g_hWnd, "���!", "", MB_OK);
}