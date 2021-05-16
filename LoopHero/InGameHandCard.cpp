#include "InGameHandCard.h"
#include "Card.h"
#include "Deck.h"
#include "UIHorizontalScroll.h"
#include "UIProgressBar.h"
#include "UISprite.h"

void InGameHandCard::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);
	moveSpeed = 200;
	initPos = pos;

	//vHandCards.reserve(13);
	//vDropCards.reserve(13);

	UIHorizontalScroll* lpUIHScroll = GameUI::CreateUI<UIHorizontalScroll>(this);
	lpUIHScroll->Init(UI_ANCHOR::LEFT_BOTTOM, { 0.0f, 0.0f }, WINSIZE_WIDTH - 296, 58 * 2, HSCROLL_ALIGN::LEFT, HS_ARGS_INSERT::AFTER, 18);
	lpHScrollView = lpUIHScroll;

	lpProgressBar = GameUI::CreateUI<UIProgressBar>(this);
	lpProgressBar->Init(UI_ANCHOR::LEFT_BOTTOM, { 100.0f, 400.0f }, 50, 4, UI_BAR_TYPE::HORIZON, "battle_unit_statusbar_action", "battle_unit_statusbar_hp");

	auto func = bind([](GameUI* target)->float {return (float)target->GetChildCount(); }, lpHScrollView);
	lpProgressBar->SetTrackingVariable(func, 18);
}

void InGameHandCard::Release()
{
	GameUI::Release();
}

void InGameHandCard::Update(float deltaTime)
{
	//if (KeyManager::GetSingleton()->IsKeyOnceDown('M'))
	//{
	//	vHandCards.push_back(GameData::GetSingleton()->PickCard());
	//}

	if (KeyManager::GetSingleton()->IsKeyOnceDown('I'))
	{
		for (int i = 0; i < 20; ++i)
		{
			UISprite* lpSprite = GameUI::CreateUI<UISprite>();
			lpSprite->Init(UI_ANCHOR::LEFT_TOP, { 0.0f, 0.0f }, 41 * 2, 58 * 2);
			lpSprite->SetObject(GameData::GetSingleton()->PickCard());
			lpHScrollView->AddChildUI(lpSprite);
		}
	}

	if (KeyManager::GetSingleton()->IsKeyOnceDown('O'))
	{
		((UIHorizontalScroll*)lpHScrollView)->RemoveChildUI(rand() % 10);
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

	GameUI::Update(deltaTime);
}

void InGameHandCard::Render(HDC hdc)
{
	GameUI::Render(hdc);
}
