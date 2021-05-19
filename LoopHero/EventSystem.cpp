#include "EventSystem.h"
#include "EventTrigger.h"
#include "GameUI.h"

void EventSystem::InitEventData()
{
	lpData->type = EVENT_TYPE::NONE;
	lpData->isUsed = false;
	lpData->lpTarget = nullptr;
	auto compare = [](GameUI* a, GameUI* b) { if (a->GetDepth() == b->GetDepth()) { return a > b; } else { return a->GetDepth() > b->GetDepth(); } };
	qlpGameUIByDepth = decltype(qlpGameUIByDepth)(compare);
}

GameUI* EventSystem::FindDispatcherUI(GameUI* lpCurrUI)
{
	RECT rc = lpCurrUI->GetRect();
	GameUI* lpEventTarget = nullptr, *lpFindTarget;
	if (PtInRect(&rc, lpData->point))
	{
		lpEventTarget = lpCurrUI;
		qlpGameUIByDepth.push(lpCurrUI);
		vector<GameUI*> vLpChild = lpCurrUI->GetChilds();
		for (int i = vLpChild.size() - 1; i > -1; --i)
		{
			if (!vLpChild[i]->IsVisible()) continue;

			lpFindTarget = FindDispatcherUI(vLpChild[i]);
			if (lpFindTarget && lpEventTarget->GetDepth() < lpFindTarget->GetDepth()) lpEventTarget = lpFindTarget;
		}
	}
	return lpEventTarget;
}

HRESULT EventSystem::Init()
{
	lpData = new EventData();
	return S_OK;
}

void EventSystem::Release()
{
	if (lpData)
	{
		delete lpData;
		lpData = nullptr;
	}
}

void EventSystem::Update(float deltaTime)
{
	if (KeyManager::GetSingleton()->IsKeyOnceDown(VK_LBUTTON))
	{
		InitEventData();
		lpData->type = EVENT_TYPE::CLICK;
		lpData->point = KeyManager::GetSingleton()->GetMousePoint();
		lpData->deltaTime = deltaTime;

		if (FindDispatcherUI(lpGameUI))
		{
			while (!lpData->isUsed && !qlpGameUIByDepth.empty())
			{
				lpData->isUsed = true;
				lpData->lpTarget = qlpGameUIByDepth.top();
				lpData->lpTarget->OnClick(*lpData);
				qlpGameUIByDepth.pop();
				if (!lpData->isUsed) lpData->lpTarget = nullptr;
			}
		}
	}
	else if (KeyManager::GetSingleton()->IsKeyOnceUp(VK_LBUTTON))
	{
		if (lpData->type == EVENT_TYPE::DRAG && lpData->lpTarget)
		{
			lpData->type = EVENT_TYPE::END_DRAG;
			lpData->point = KeyManager::GetSingleton()->GetMousePoint();
			lpData->deltaTime = deltaTime;
			lpData->lpTarget->OnEndDrag(*lpData);

			if (FindDispatcherUI(lpGameUI))
			{
				while (!lpData->isUsed && !qlpGameUIByDepth.empty())
				{
					lpData->isUsed = true;
					qlpGameUIByDepth.top()->OnDrop(*lpData);
					qlpGameUIByDepth.pop();
				}
			}

			InitEventData();
		}
	}
	else if (KeyManager::GetSingleton()->IsKeyStayDown(VK_LBUTTON))
	{
		if (lpData->type == EVENT_TYPE::CLICK)
		{
			if (lpData->lpTarget)
			{
				// 클릭상태였으면서
				// 드레그 처음시작
				lpData->type = EVENT_TYPE::BEGIN_DRAG;
				lpData->point = KeyManager::GetSingleton()->GetMousePoint();
				lpData->deltaTime = deltaTime;
				lpData->lpTarget->OnBeginDrag(*lpData);
			}
		}
		else if ((lpData->type == EVENT_TYPE::BEGIN_DRAG || lpData->type == EVENT_TYPE::DRAG)
			&& lpData->lpTarget)
		{
			// 드래그 시작 상태였거나 그래그 중이면서 Target이 존재
			lpData->type = EVENT_TYPE::DRAG;
			lpData->point = KeyManager::GetSingleton()->GetMousePoint();
			lpData->deltaTime = deltaTime;
			lpData->lpTarget->OnDrag(*lpData);
		}
	}
}

void EventSystem::Render(HDC hdc)
{
	SetBkMode(hdc, OPAQUE);
	if (lpData)
	{
		if (lpData->lpTarget)
		{
			string text = to_string(lpData->point.x) + ", " + to_string(lpData->point.y) + " : " + to_string((__int64)(lpData->lpTarget)) + " / " + to_string((int)lpData->type) + " / " + to_string(lpData->deltaTime);
			TextOut(hdc, 300, 10, text.c_str(), text.length());
			text = to_string(((GameUI*)lpData->lpTarget)->GetPos().x) + ", " + to_string(((GameUI*)lpData->lpTarget)->GetPos().y);
			TextOut(hdc, 300, 25, text.c_str(), text.length());
		}
	}
	SetBkMode(hdc, TRANSPARENT);
}

