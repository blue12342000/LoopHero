#include "InGameRightMenu.h"
#include "Image.h"
#include "InGameEventTimer.h"
#include "UIButton.h"
#include "UIItemSlot.h"
#include "UIHorizontalScroll.h"
#include "UIGrid.h"
#include "UISprite.h"
#include "UITextField.h"
#include "EquipItem.h"
#include "Unit.h"
#include "Trait.h"
#include "LoopHero.h"

void InGameRightMenu::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);

	lpBackground = ImageManager::GetSingleton()->FindImage("ingame_info_background");

	lpHeroEquip = GameUI::CreateUI<UIGrid>(this);
	lpHeroEquip->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 5.0f * 2, 27.0f * 2 }, 104 * 2, 77 * 2, 3, 4, { 2, 2 });

	UIItemSlot* lpItemSlot1 = GameUI::CreateUI<UIItemSlot>();
	lpItemSlot1->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 0, 0 }, 25 * 2, 25 * 2, EQUIP_PARTS::SWORD);
	lpHeroEquip->SetItemObject(0, 0, lpItemSlot1);

	UIItemSlot* lpItemSlot2 = GameUI::CreateUI<UIItemSlot>();
	lpItemSlot2->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 0, 0 }, 25 * 2, 25 * 2, EQUIP_PARTS::HELMET);
	lpHeroEquip->SetItemObject(0, 1, lpItemSlot2);
	
	UIItemSlot* lpItemSlot3 = GameUI::CreateUI<UIItemSlot>();
	lpItemSlot3->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 0, 0 }, 25 * 2, 25 * 2, EQUIP_PARTS::RING);
	lpHeroEquip->SetItemObject(1, 0, lpItemSlot3);
	
	UIItemSlot* lpItemSlot4 = GameUI::CreateUI<UIItemSlot>();
	lpItemSlot4->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 0, 0 }, 25 * 2, 25 * 2, EQUIP_PARTS::SHIELD);
	lpHeroEquip->SetItemObject(2, 0, lpItemSlot4);
	
	UIItemSlot* lpItemSlot5 = GameUI::CreateUI<UIItemSlot>();
	lpItemSlot5->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 0, 0 }, 25 * 2, 25 * 2, EQUIP_PARTS::ARMOR);
	lpHeroEquip->SetItemObject(2, 1, lpItemSlot5);

	lpHScroll = GameUI::CreateUI<UIHorizontalScroll>(this);
	lpHScroll->Init(UI_ANCHOR::RIGHT_TOP, POINTFLOAT{ 8.0f * 2, 129.0f * 2 }, 100 * 2 - 4, 23 * 2 * 3 + 4 * 2, HSCROLL_ALIGN::LEFT, HS_ARGS_INSERT::BEFORE, 12);
	lpHScroll->SetMultiLineType(HSCROLL_MULTILINE::ZIGZAG, 4);
	lpHScroll->SetHScrollControl(HSCROLL_ITEM_CONTROL::DRAG_SWAP);

	lpButton = GameUI::CreateUI<UIButton>(this);
	lpButton->Init(UI_ANCHOR::LEFT_BOTTOM, POINTFLOAT{ 3.0f * 2, 4.0f * 2 }, 35 * 2, 29 * 2, UI_BUTTON_TYPE::BUTTON);
	lpButton->SetButtonImage("button_exit");
	//lpButton->PushBackFunc(bind([](HWND hWnd) {MessageBox(hWnd, "테스트", "클릭", MB_OK); }, g_hWnd));

	UITextField* lpTextField = GameUI::CreateUI<UITextField>(this);
	lpTextField->Init(UI_ANCHOR::RIGHT_BOTTOM, { 105.0f * 2, 145.0f * 2 }, 200, 50);
	lpTextField->SetFont(UI_TEXT_HALIGN::CENTER, UI_TEXT_VALIGN::MIDDLE, UI_TEXT_STYLE::BOLD, UI_TEXT_LINE::MULTI, 20, RGB(0, 0, 0), "돋움");
	lpTextField->SetEventCatch(EVENT_CATCH::BLOCK_PASS);
}

void InGameRightMenu::Update(float deltaTime)
{
	if (KeyManager::GetSingleton()->IsKeyOnceDown('P'))
	{
		EquipItem* lpEquipItem = GameData::GetSingleton()->GetUnit()->GetTrait()->CreateEquip();
		if (lpEquipItem)
		{
			UISprite* lpSprite = GameUI::CreateUI<UISprite>();
			lpSprite->Init(UI_ANCHOR::LEFT_TOP, { 0.0f, 0.0f }, 23 * 2, 23 * 2);
			lpSprite->SetGameObject(lpEquipItem);
			lpSprite->SetEventCatch(EVENT_CATCH::BLOCK_PASS);
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
