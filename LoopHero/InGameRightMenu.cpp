#include "InGameRightMenu.h"
#include "Image.h"
#include "InGameEventTimer.h"
#include "UIButton.h"
#include "UIItemSlot.h"
#include "UIHorizontalScroll.h"
#include "UIGrid.h"
#include "UISprite.h"
#include "UIInGameInfo.h"
#include "UIProgressBar.h"
#include "EquipItem.h"
#include "Unit.h"
#include "Trait.h"
#include "LoopHero.h"

void InGameRightMenu::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpBackground = ImageManager::GetSingleton()->FindImage("ingame_info_background");

	lpHeroEquip = GameUI::Create<UIGrid>(this);
	lpHeroEquip->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 5.0f * 2, 27.0f * 2 }, 104 * 2, 77 * 2, 3, 4, { 2, 2 });

	Unit* lpUnit = GameData::GetSingleton()->GetUnit();
	auto mSlotEquip = lpUnit->GetTrait()->GetUnitSlot();

	if (mSlotEquip.find(UNIT_SLOT::RIGHT_HAND) != mSlotEquip.end())
	{
		UIItemSlot* lpItemSlot1 = GameUI::Create<UIItemSlot>();
		lpItemSlot1->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 0, 0 }, 25 * 2, 25 * 2, UNIT_SLOT::RIGHT_HAND, mSlotEquip[UNIT_SLOT::RIGHT_HAND]);
		lpHeroEquip->SetItemObject(0, 0, lpItemSlot1);
	} 

	if (mSlotEquip.find(UNIT_SLOT::HELMET) != mSlotEquip.end())
	{
		UIItemSlot* lpItemSlot2 = GameUI::Create<UIItemSlot>();
		lpItemSlot2->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 0, 0 }, 25 * 2, 25 * 2, UNIT_SLOT::HELMET, mSlotEquip[UNIT_SLOT::HELMET]);
		lpHeroEquip->SetItemObject(0, 1, lpItemSlot2);
	}
	
	if (mSlotEquip.find(UNIT_SLOT::LEFT_RING) != mSlotEquip.end())
	{
		UIItemSlot* lpItemSlot3 = GameUI::Create<UIItemSlot>();
		lpItemSlot3->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 0, 0 }, 25 * 2, 25 * 2, UNIT_SLOT::LEFT_RING, mSlotEquip[UNIT_SLOT::LEFT_RING]);
		lpHeroEquip->SetItemObject(1, 0, lpItemSlot3);
	}
	
	if (mSlotEquip.find(UNIT_SLOT::LEFT_HAND) != mSlotEquip.end())
	{
		UIItemSlot* lpItemSlot4 = GameUI::Create<UIItemSlot>();
		lpItemSlot4->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 0, 0 }, 25 * 2, 25 * 2, UNIT_SLOT::LEFT_HAND, mSlotEquip[UNIT_SLOT::LEFT_HAND]);
		lpHeroEquip->SetItemObject(2, 0, lpItemSlot4);
	}
	
	if (mSlotEquip.find(UNIT_SLOT::ARMOR) != mSlotEquip.end())
	{
		UIItemSlot* lpItemSlot5 = GameUI::Create<UIItemSlot>();
		lpItemSlot5->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 0, 0 }, 25 * 2, 25 * 2, UNIT_SLOT::ARMOR, mSlotEquip[UNIT_SLOT::ARMOR]);
		lpHeroEquip->SetItemObject(2, 1, lpItemSlot5);
	}

	lpHScroll = GameUI::Create<UIHorizontalScroll>(this);
	lpHScroll->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 8.0f * 2, 129.0f * 2 }, 100 * 2 - 4, 23 * 2 * 3 + 4 * 2, HSCROLL_ALIGN::LEFT, HS_ARGS_INSERT::BEFORE, 12);
	lpHScroll->SetMultiLineType(HSCROLL_MULTILINE::ZIGZAG, 4);
	lpHScroll->SetHScrollControl(HSCROLL_ITEM_CONTROL::DRAG_SWAP);

	lpHeroHpBar = GameUI::Create<UIProgressBar>(this);
	lpHeroHpBar->Init(UI_ANCHOR::RIGHT_TOP, { 20.0f * 2, 123.0f * 2 }, 75 * 2, 7 * 2, UI_BAR_TYPE::HORIZON, "", "battle_unit_statusbar_hp");
	lpHeroHpBar->SetTrackingData(bind(&Unit::GetCurrHp, GameData::GetSingleton()->GetUnit()));
	lpHeroHpBar->SetTrackingMaxData(bind(&Unit::GetStatus, GameData::GetSingleton()->GetUnit(), UNIT_STATUS::MAX_HP));

	lpButton = GameUI::Create<UIButton>(this);
	lpButton->Init(UI_ANCHOR::LEFT_BOTTOM, POINTFLOAT{ 3.0f * 2, 4.0f * 2 }, 35 * 2, 29 * 2, UI_BUTTON_TYPE::BUTTON);
	lpButton->SetButtonImage("button_exit");

	lpGameInfo = GameUI::Create<UIInGameInfo>(this);
	lpGameInfo->Init(UI_ANCHOR::RIGHT_BOTTOM, { 15.0f, 10.0f }, 195, 285);

	AddEventHandler("DropEquip", bind(&InGameRightMenu::DropEquip, this, placeholders::_1));
}

void InGameRightMenu::Update(float deltaTime)
{
	if (KeyManager::GetSingleton()->IsKeyOnceDown('P'))
	{
		DropEquip(GameData::GetSingleton()->GetUnit());
	}

	GameUI::Update(deltaTime);
}

void InGameRightMenu::Render(HDC hdc)
{
	if (isVisible) lpBackground->Render(hdc, rc.left, rc.top, 0);

	GameUI::Render(hdc);
}

void InGameRightMenu::DropEquip(ObserverHandler* lpCaller)
{
	if (typeid(*lpCaller) == typeid(Unit))
	{
		EquipItem* lpEquipItem = ((Unit*)lpCaller)->GetTrait()->CreateEquip();
		if (lpEquipItem)
		{
			UISprite* lpSprite = GameUI::Create<UISprite>();
			lpSprite->Init(UI_ANCHOR::LEFT_TOP, { 0.0f, 0.0f }, 23 * 2, 23 * 2);
			lpSprite->SetGameObject(lpEquipItem);
			lpSprite->SetEventCatch(EVENT_CATCH::BLOCK_PASS);
			lpHScroll->AddChild(lpSprite);
		}
	}
}
