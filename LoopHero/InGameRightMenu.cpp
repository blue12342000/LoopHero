#include "InGameRightMenu.h"
#include "Image.h"
#include "InGameEventTimer.h"
#include "UIHorizontalScroll.h"
#include "UISprite.h"
#include "LoopHero.h"

void InGameRightMenu::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpBackground = ImageManager::GetSingleton()->FindImage("ingame_info_background");

	GameUI* lpEvent = GameUI::CreateUI<InGameEventTimer>();
	lpEvent->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ WINSIZE_WIDTH - 121.0f * 2, 0.0f }, 121 * 2, 27 * 2);
	this->AddChildUI(lpEvent);

	for (int i=0;i<3;++i)
	{
		lpHScroll[i] = GameUI::CreateUI<UIHorizontalScroll>();
		lpHScroll[i]->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 8.0f * 2, 129.0f * 2 + 50 * i }, 100 * 2 - 4, 23 * 2, (HSCROLL_ALIGN)(i % 2), 4);

		for (int l = 0; l < 4; ++l)
		{
			UISprite* lpSprite = GameUI::CreateUI<UISprite>();
			lpSprite->Init(UI_ANCHOR::LEFT_TOP, { 0.0f, 0.0f }, 23 * 2, 23 * 2);
			lpHScroll[i]->AddChildUI(lpSprite);
		}

		//auto func = bind(&UIHorizontalScroll::AddChildUI, lpHScroll[i + 1], placeholders::_1);
		//lpHScroll[i]->SetOnRemove(func);

		this->AddChildUI(lpHScroll[i]);
	}
}

void InGameRightMenu::Update(float deltaTime)
{
	GameUI::Update(deltaTime);
}

void InGameRightMenu::Render(HDC hdc)
{
	if (isVisible) lpBackground->Render(hdc, rc.left, rc.top, 0);

	GameUI::Render(hdc);
}
