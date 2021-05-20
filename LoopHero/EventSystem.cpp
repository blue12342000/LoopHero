#include "EventSystem.h"
#include "EventTrigger.h"
#include "GameUI.h"

void EventSystem::InitEventData(EventData& data)
{
	data.type = EVENT_TYPE::NONE;
	data.isUsed = false;
	data.lpTarget = nullptr;
}

GameUI* EventSystem::FindDispatcherUI(GameUI* lpCurrUI)
{
	RECT rc = lpCurrUI->GetRect();
	GameUI* lpEventTarget = nullptr, *lpFindTarget;
	if (PtInRect(&rc, lpPoint->point))
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
	lpPoint = new EventData();
	lpBase = new EventData();
	lpCompareFunc = [](GameUI* a, GameUI* b) { if (a->GetDepth() == b->GetDepth()) { return a < b; } else { return a->GetDepth() < b->GetDepth(); } };
	return S_OK;
}

void EventSystem::Release()
{
	if (lpPoint)
	{
		delete lpPoint;
		lpPoint = nullptr;
	}

	if (lpBase)
	{
		delete lpBase;
		lpBase = nullptr;
	}
}

void EventSystem::Update(float deltaTime)
{
	qlpGameUIByDepth = decltype(qlpGameUIByDepth)(lpCompareFunc);

	lpPoint->point = KeyManager::GetSingleton()->GetMousePoint();
	if ((lpFindTop = FindDispatcherUI(lpGameUI)) != nullptr && lpFindTop != lpBase->lpTarget)
	{
		lpBase->isUsed = true;
		lpBase->deltaTime = deltaTime;
		lpBase->point = lpPoint->point;
		if (lpBase->lpTarget)
		{
			lpBase->type = EVENT_TYPE::MOUSE_OUT;
			lpBase->lpTarget->OnMouseOut(*lpBase);
		}

		lpBase->lpTarget = lpFindTop;
		lpBase->type = EVENT_TYPE::MOUSE_ENTER;
		lpBase->lpTarget->OnMouseEnter(*lpBase);
	}
	else if (lpBase->lpTarget)
	{
		lpBase->deltaTime = deltaTime;
		lpBase->type = EVENT_TYPE::MOUSE_OVER;
		lpBase->point = lpPoint->point;
		lpBase->lpTarget->OnMouseOver(*lpBase);
	}

	if (KeyManager::GetSingleton()->IsKeyOnceDown(VK_LBUTTON))
	{
		InitEventData(*lpPoint);
		lpPoint->type = EVENT_TYPE::CLICK;
		lpPoint->deltaTime = deltaTime;

		while (!lpPoint->isUsed && !qlpGameUIByDepth.empty())
		{
			lpPoint->isUsed = true;
			lpPoint->lpTarget = qlpGameUIByDepth.top();
			lpPoint->lpTarget->OnClick(*lpPoint);
			qlpGameUIByDepth.pop();
			if (!lpPoint->isUsed) lpPoint->lpTarget = nullptr;
		}
	}
	else if (KeyManager::GetSingleton()->IsKeyOnceUp(VK_LBUTTON))
	{
		if (lpPoint->type == EVENT_TYPE::DRAG && lpPoint->lpTarget)
		{
			lpPoint->type = EVENT_TYPE::END_DRAG;
			lpPoint->deltaTime = deltaTime;
			lpPoint->lpTarget->OnEndDrag(*lpPoint);

			while (!lpPoint->isUsed && !qlpGameUIByDepth.empty())
			{
				lpPoint->isUsed = true;
				qlpGameUIByDepth.top()->OnDrop(*lpPoint);
				qlpGameUIByDepth.pop();
			}

			InitEventData(*lpPoint);
		}
	}
	else if (KeyManager::GetSingleton()->IsKeyStayDown(VK_LBUTTON))
	{
		if (lpPoint->type == EVENT_TYPE::CLICK)
		{
			if (lpPoint->lpTarget)
			{
				// 클릭상태였으면서
				// 드레그 처음시작
				lpPoint->type = EVENT_TYPE::BEGIN_DRAG;
				lpPoint->deltaTime = deltaTime;
				lpPoint->lpTarget->OnBeginDrag(*lpPoint);
			}
		}
		else if ((lpPoint->type == EVENT_TYPE::BEGIN_DRAG || lpPoint->type == EVENT_TYPE::DRAG)
			&& lpPoint->lpTarget)
		{
			// 드래그 시작 상태였거나 그래그 중이면서 Target이 존재
			lpPoint->type = EVENT_TYPE::DRAG;
			lpPoint->deltaTime = deltaTime;
			lpPoint->lpTarget->OnDrag(*lpPoint);
		}
	}
}

void EventSystem::Render(HDC hdc)
{
	SetBkMode(hdc, OPAQUE);
	if (lpPoint)
	{
		if (lpPoint->lpTarget)
		{
			string text = to_string(lpPoint->point.x) + ", " + to_string(lpPoint->point.y) + " : " + to_string((__int64)(lpPoint->lpTarget)) + " / " + to_string((int)lpPoint->type) + " / " + to_string(lpPoint->deltaTime);
			TextOut(hdc, 300, 10, text.c_str(), text.length());
			text = to_string(((GameUI*)lpPoint->lpTarget)->GetPos().x) + ", " + to_string(((GameUI*)lpPoint->lpTarget)->GetPos().y);
			TextOut(hdc, 300, 25, text.c_str(), text.length());
		}
	}
	SetBkMode(hdc, TRANSPARENT);
}

