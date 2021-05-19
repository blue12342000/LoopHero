#include "InGameRightMenu.h"
#include "Image.h"
#include "InGameEventTimer.h"
#include "UIItemSlot.h"
#include "UIHorizontalScroll.h"
#include "UISprite.h"
#include "EquipItem.h"
#include "Unit.h"
#include "TileTable.h"
#include "LoopHero.h"

void InGameRightMenu::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpBackground = ImageManager::GetSingleton()->FindImage("ingame_info_background");

	GameUI* lpEvent = GameUI::CreateUI<InGameEventTimer>(this);
	lpEvent->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ WINSIZE_WIDTH - 121.0f * 2, 0.0f }, 121 * 2, 27 * 2);

	lpItemSlot = GameUI::CreateUI<UIHorizontalScroll>(this);
	lpItemSlot->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 5.0f * 2, 27.0f * 2 }, 104 * 2, 77 * 2, HSCROLL_ALIGN::LEFT, HS_ARGS_INSERT::BEFORE, 12);
	lpItemSlot->SetMultiLineType(HSCROLL_MULTILINE::ALIGN, 2);

	lpHScroll = GameUI::CreateUI<UIHorizontalScroll>(this);
	lpHScroll->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 8.0f * 2, 129.0f * 2 }, 100 * 2 - 4, 23 * 2 * 3 + 4 * 2, HSCROLL_ALIGN::LEFT, HS_ARGS_INSERT::BEFORE, 12);
	lpHScroll->SetMultiLineType(HSCROLL_MULTILINE::ZIGZAG, 4);
}

void InGameRightMenu::Update(float deltaTime)
{
	if (KeyManager::GetSingleton()->IsKeyOnceDown('P'))
	{
		EquipItem* lpEquipItem = EquipItem::CreateEquip(GameData::GetSingleton()->GetUnit()->GetTraits());
		if (lpEquipItem)
		{
			UISprite* lpSprite = GameUI::CreateUI<UISprite>();
			lpSprite->Init(UI_ANCHOR::LEFT_TOP, { 0.0f, 0.0f }, 23 * 2, 23 * 2);
			lpSprite->SetObject(lpEquipItem);
			lpHScroll->AddChildUI(lpSprite);
		}
	}

	GameUI::Update(deltaTime);
}

void InGameRightMenu::Render(HDC hdc)
{
	if (isVisible) lpBackground->Render(hdc, rc.left, rc.top, 0);

	GameUI::Render(hdc);
}
