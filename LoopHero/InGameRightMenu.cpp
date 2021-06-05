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
#include "UIItemInfo.h"
#include "UITextField.h"
#include "EquipItem.h"
#include "Unit.h"
#include "Trait.h"
#include "LoopHero.h"

void InGameRightMenu::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpBackground = ImageManager::GetSingleton()->FindImage("ingame_info_background");

	lpLoopLevelLabel = GameUI::Create<UITextField>(this);
	lpLoopLevelLabel->Init(UI_ANCHOR::TOP_MIDDLE, { 32.0f, 20.0f }, 50, 25);
	lpLoopLevelLabel->SetFont(UI_TEXT_HALIGN::CENTER, UI_TEXT_VALIGN::MIDDLE, UI_TEXT_LINE::SINGLE, 22, RGB(255, 255, 255), "Bµ¸¿ò");

	lpHeroHpBar = GameUI::Create<UIProgressBar>(this);
	lpHeroHpBar->Init(UI_ANCHOR::RIGHT_TOP, { 20.0f * 2, 241.0f }, 75 * 2, 6 * 2, UI_BAR_TYPE::HORIZON, "", "battle_unit_statusbar_hp");

	lpHeroHpLabel = GameUI::Create<UITextField>(this);
	lpHeroHpLabel->Init(UI_ANCHOR::RIGHT_TOP, { 20.0f * 2, 234.0f }, 75 * 2, 6 * 2);
	lpHeroHpLabel->SetFont(UI_TEXT_HALIGN::CENTER, UI_TEXT_VALIGN::MIDDLE, UI_TEXT_LINE::SINGLE, 11, RGB(255, 255, 255), "B³ª´®¹Ù¸¥°íµñ");
	lpHeroHpLabel->SetStyle(UI_TEXT_STYLE::OUTLINE);
	
	lpHeroHpBar->SetLabel(lpHeroHpLabel);
	
	lpHeroEquip = GameUI::Create<UIGrid>(this);
	lpHeroEquip->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 5.0f * 2, 27.0f * 2 }, 104 * 2, 77 * 2, 3, 4, { 2, 2 });

	Unit* lpUnit = GameData::GetSingleton()->GetUnit();
	if (lpUnit)
	{
		lpHeroHpBar->SetTrackingData(bind(&Unit::GetCurrHp, GameData::GetSingleton()->GetUnit()));
		lpHeroHpBar->SetTrackingMaxData(bind(&Unit::GetStatus, GameData::GetSingleton()->GetUnit(), UNIT_STATUS::MAX_HP));

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
	}

	lpHScroll = GameUI::Create<UIHorizontalScroll>(this);
	lpHScroll->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 8.0f * 2, 129.0f * 2 }, 100 * 2 - 4, 23 * 2 * 3 + 4 * 2, HSCROLL_ALIGN::LEFT, HS_ARGS_INSERT::BEFORE, 12);
	lpHScroll->SetMultiLineType(HSCROLL_MULTILINE::ZIGZAG, 4);
	lpHScroll->SetHScrollControl(HSCROLL_ITEM_CONTROL::DRAG_SWAP);

	lpButton = GameUI::Create<UIButton>(this);
	lpButton->Init(UI_ANCHOR::LEFT_BOTTOM, POINTFLOAT{ 3.0f * 2, 4.0f * 2 }, 35 * 2, 29 * 2, UI_BUTTON_TYPE::BUTTON);
	lpButton->SetButtonImage("button_exit");

	lpGameInfo = GameUI::Create<UIInGameInfo>(this);
	lpGameInfo->Init(UI_ANCHOR::RIGHT_BOTTOM, { 15.0f, 10.0f }, 195, 285);

	lpItemInfo = nullptr;
	vCompareItems.clear();

	CreateAnimController("ingame_info");

	AddEventHandler("DropEquip", bind(&InGameRightMenu::DropEquip, this, placeholders::_1));
	AddEventHandler("ViewInfo", bind(&InGameRightMenu::OpenEquipLayer, this, placeholders::_1));
	AddEventHandler("HideInfo", bind(&InGameRightMenu::CloseEquipLayer, this, placeholders::_1));
}

void InGameRightMenu::Update(float deltaTime)
{
	//if (KeyManager::GetSingleton()->IsKeyOnceDown('P'))
	//{
	//	DropEquip(GameData::GetSingleton()->GetUnit());
	//}

	lpLoopLevelLabel->SetText(to_string(GameData::GetSingleton()->GetLoopLevel()));

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
			lpSprite->SetWorldPos(POINT{ WINSIZE_WIDTH / 2, WINSIZE_HEIGHT / 2 });
			lpSprite->Refresh();
			lpHScroll->AddChild(lpSprite);
		}
	}
}

void InGameRightMenu::OpenEquipLayer(ObserverHandler* lpCaller)
{
	if (typeid(*lpCaller) == typeid(EquipItem))
	{
		EquipItem* lpEquipItem = (EquipItem*)lpCaller;
		RECT invenRc = move(lpHScroll->GetRect());
		if (PtInRect(&invenRc, KeyManager::GetSingleton()->GetMousePoint()))
		{
			if (!lpItemInfo)
			{
				lpItemInfo = GameUI::Create<UIItemInfo>(this);
				lpItemInfo->Init(UI_ANCHOR::RIGHT_TOP, { 115.0f * 2, 110 * 2.0f }, 110 * 2, 80 * 2);
			}
			lpItemInfo->SetVisible(true);
			lpItemInfo->SetEquipItem(lpEquipItem);
		}

		Unit* lpUnit = GameData::GetSingleton()->GetUnit();
		auto mEquips = lpUnit->GetEquips();
		int count = -1;
		for (const auto& pair : mEquips)
		{
			if (pair.second.parts == lpEquipItem->GetParts() && pair.second.lpEquip)
			{
				++count;
				if (vCompareItems.size() <= count)
				{
					vCompareItems.push_back(nullptr);
					vCompareItems[count] = GameUI::Create<UIItemInfo>(this);
					vCompareItems[count]->Init(UI_ANCHOR::RIGHT_TOP, { 115.0f * 2 + 230.0f * count, 0.0f }, 110 * 2, 80 * 2);
				}

				vCompareItems[count]->SetVisible(true);
				vCompareItems[count]->SetEquipItem(pair.second.lpEquip);
				vCompareItems[count]->SetTag("ÀåºñÁß");
			}
		}
	}
	else
	{
		CloseEquipLayer(this);
	}
}

void InGameRightMenu::CloseEquipLayer(ObserverHandler* lpCaller)
{
	if(lpItemInfo) lpItemInfo->SetVisible(false);
	for (const auto& lpItemUI : vCompareItems)
	{
		lpItemUI->SetVisible(false);
	}
}
