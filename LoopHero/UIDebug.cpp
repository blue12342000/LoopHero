#include "UIDebug.h"
#include "UITextField.h"
#include "ImageManager.h"
#include "Image.h"
#include "Timer.h"
#include "Utill.h"

void UIDebug::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);
	SetEventCatch(EVENT_CATCH::BLOCK_PASS);

	hDrawDC = GetDC(g_hWnd);
	lpBackground = ImageManager::GetSingleton()->FindImage("layer_background");

	lpLogText = GameUI::Create<UITextField>(this);
	lpLogText->Init(UI_ANCHOR::LEFT_TOP, POINTFLOAT{ 20.0f, 25.0f }, width, height - 50);
	lpLogText->SetFont(UI_TEXT_HALIGN::LEFT, UI_TEXT_VALIGN::TOP, UI_TEXT_LINE::MULTI, 16, RGB(255, 255, 255), "B³ª´®¹Ù¸¥°íµñ");
	lpLogText->SetEventCatch(EVENT_CATCH::BLOCK_PASS);

	AddEventHandler("OpenDebugInfo", bind(&UIDebug::OpenDebugInfo, this, placeholders::_1));
}

void UIDebug::Release()
{
	ReleaseDC(g_hWnd, hDrawDC);
	hDrawDC = NULL;
	lpLogText = nullptr;
	lpDebugNode = nullptr;
	GameUI::Release();
}

void UIDebug::Update(float deltaTime)
{
	if (lpLogText)
	{
		lpLogText->SetText("FPS\t:\t" + to_string(Timer::FPS));
		if (lpDebugNode)
		{
			lpLogText->AppendText("\n" + lpDebugNode->ToString());
		}
		RECT inRect = {0, 0, 0, 0};
		DrawText(hDrawDC, lpLogText->GetText().c_str(), lpLogText->GetText().length(), &inRect, DT_CALCRECT);
		width = (inRect.right - inRect.left) + 50;
		height = (inRect.bottom - inRect.top) + 50;
		if (width < 250) width = 250;
		if (height < 50) height = 50;

		lpLogText->SetHeight(height - 50);
	}

	GameUI::Update(deltaTime);
}

void UIDebug::Render(HDC hdc)
{
	if (lpBackground) lpBackground->LoopRender(hdc, POINT{ rc.left, rc.top }, width, height, 0, IMAGE_ALIGN::LEFT_TOP);

	GameUI::Render(hdc);
}

void UIDebug::OpenDebugInfo(ObserverHandler* lpTarget)
{
	lpDebugNode = dynamic_cast<GameNode*>(lpTarget);
	isVisible = (lpDebugNode != nullptr);
}

void UIDebug::OnClick(EventData& data)
{
	if (isVisible) isVisible = false;
}
