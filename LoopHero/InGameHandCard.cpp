#include "InGameHandCard.h"
#include "Card.h"
#include "Deck.h"

void InGameHandCard::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);
	moveSpeed = 200;
	initPos = pos;

	vHandCards.reserve(13);
	vDropCards.reserve(13);
}

void InGameHandCard::Update(float deltaTime)
{
	if (KeyManager::GetSingleton()->IsKeyOnceDown('M'))
	{
		vHandCards.push_back(GameData::GetSingleton()->PickCard());
	}

	if (PtInRect(&rc, KeyManager::GetSingleton()->GetMousePoint()))
	{
		pos.y += moveSpeed * deltaTime;
		if (pos.y - initPos.y > 40) pos.y = initPos.y + 40;

		Refresh();
	}
	else
	{
		pos.y -= moveSpeed * deltaTime;
		if (pos.y - initPos.y < 0) pos.y = initPos.y;

		Refresh();
	}

	for (int i = 0; i < vHandCards.size(); ++i)
	{
		
	}
}

void InGameHandCard::Render(HDC hdc)
{
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

}
