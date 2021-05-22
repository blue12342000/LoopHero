#include "EventSystem.h"
#include "EventTrigger.h"
#include "GameUI.h"
#include "GameObject.h"

void EventSystem::InitEventData(EventData& data)
{
	data.message.reset();
	if (KeyManager::GetSingleton()->IsKeyOnceDown(VK_LBUTTON))
	{
		data.message[(int)EVENT_TYPE::CLICK] = true;
	}
	else if (KeyManager::GetSingleton()->IsKeyOnceUp(VK_LBUTTON))
	{
		if (data.lpDragTarget)
		{
			data.message[(int)EVENT_TYPE::END_DRAG] = true;
			data.message[(int)EVENT_TYPE::DROP] = true;
		}
	}
	else if (KeyManager::GetSingleton()->IsKeyStayDown(VK_LBUTTON))
	{
		if (data.isDragging) data.message[(int)EVENT_TYPE::DRAG] = true;
		else data.message[(int)EVENT_TYPE::BEGIN_DRAG] = true;
	}

	data.message[(int)EVENT_TYPE::MOUSE_ENTER] = true;
	data.message[(int)EVENT_TYPE::MOUSE_OVER] = true;
	data.message[(int)EVENT_TYPE::MOUSE_OUT] = true;

	data.type = EVENT_TYPE::NONE;
	data.slpLastTargets.clear();
	for (int i = 0; i < data.vlpTargets.size(); ++i)
	{
		data.slpLastTargets.insert(data.vlpTargets[i]);
		if (data.vlpTargets[i]->IsAllCatchEvent()) break;
	}
	data.vlpTargets.clear();
	vFindUIs.clear();
	vFindObjects.clear();
}

GameUI* EventSystem::FindDispatcherUI(vector<EventTrigger*>& vTargets, GameUI* lpCurrUI, POINT point)
{
	if (lpCurrUI == eventData.lpDragTarget) return nullptr;

	RECT rc = lpCurrUI->GetRect();
	GameUI* lpEventTarget = nullptr, *lpChildTarget;
	if (PtInRect(&rc, point))
	{
		if (lpCurrUI->IsCanCatchEvent())
		{
			lpEventTarget = lpCurrUI;
			vTargets.push_back(lpCurrUI);
		}
	}
	vector<GameUI*> vLpChild = lpCurrUI->GetChilds();
	for (int i = vLpChild.size() - 1; i > -1; --i)
	{
		if (!vLpChild[i]->IsVisible()) continue;

		lpChildTarget = FindDispatcherUI(vTargets, vLpChild[i], point);
		if (lpChildTarget)
		{
			lpEventTarget = lpChildTarget;
			break;
		}
	}
	return lpEventTarget;
}

GameObject* EventSystem::FindDispatcherObject(vector<EventTrigger*>& vTargets, GameObject* lpCurrObject, POINT point)
{
	if (lpCurrObject == eventData.lpDragTarget) return nullptr;

	RECT rc = lpCurrObject->GetRect();
	GameObject* lpEventTarget = nullptr, * lpFindTarget;
	if (PtInRect(&rc, point))
	{
		if (lpCurrObject->IsCanCatchEvent())
		{
			lpEventTarget = lpCurrObject;
			vTargets.push_back(lpCurrObject);
		}
	}
	vector<GameObject*> vLpChild = lpCurrObject->GetChilds();
	for (int i = vLpChild.size() - 1; i > -1; --i)
	{
		if (!vLpChild[i]->IsVisible()) continue;

		lpFindTarget = (GameObject*)FindDispatcherObject(vTargets, vLpChild[i], point);
		if (lpFindTarget)
		{
			lpEventTarget = lpFindTarget;
			break;
		}
	}
	return lpEventTarget;
}

void EventSystem::EventProcess()
{
	// OnMouseEnter
	if (eventData.message[(int)EVENT_TYPE::MOUSE_ENTER])
	{
		eventData.type = EVENT_TYPE::MOUSE_ENTER;
		for (int i = 0; i < eventData.vlpTargets.size(); ++i)
		{
			if (eventData.slpLastTargets.find(eventData.vlpTargets[i]) == eventData.slpLastTargets.end())
			{
				eventData.Use();
				eventData.vlpTargets[i]->OnMouseEnter(eventData);
				eventData.lpLastTarget = eventData.vlpTargets[i];
			}
			if (eventData.vlpTargets[i]->IsAllCatchEvent()) break;
		}
		eventData.Use();
	}

	// OnMouseOver
	if (eventData.message[(int)EVENT_TYPE::MOUSE_OVER])
	{
		eventData.type = EVENT_TYPE::MOUSE_OVER;
		for (int i = 0; i < eventData.vlpTargets.size(); ++i)
		{
			if (eventData.slpLastTargets.find(eventData.vlpTargets[i]) != eventData.slpLastTargets.end())
			{
				eventData.Use();
				eventData.vlpTargets[i]->OnMouseOver(eventData);
				eventData.lpLastTarget = eventData.vlpTargets[i];
			}
			if (eventData.vlpTargets[i]->IsAllCatchEvent()) break;
		}
		eventData.Use();
	}

	// OnMouseOut
	if (eventData.message[(int)EVENT_TYPE::MOUSE_OUT])
	{
		eventData.type = EVENT_TYPE::MOUSE_OUT;
		for (int i = 0; i < eventData.vlpTargets.size(); ++i)
		{
			if (eventData.slpLastTargets.find(eventData.vlpTargets[i]) != eventData.slpLastTargets.end())
			{
				eventData.slpLastTargets.erase(eventData.vlpTargets[i]);
			}
			if (eventData.vlpTargets[i]->IsAllCatchEvent()) break;
		}
		for (auto& lpRemainTarget : eventData.slpLastTargets)
		{
			eventData.Use();
			lpRemainTarget->OnMouseOut(eventData);
			eventData.lpLastTarget = lpRemainTarget;
		}

		eventData.Use();
	}
	
	// OnClick
	if (eventData.message[(int)EVENT_TYPE::CLICK])
	{
		eventData.lpLastTarget = nullptr;
		eventData.type = EVENT_TYPE::CLICK;
		for (int i = 0; i < eventData.vlpTargets.size(); ++i)
		{
			eventData.Use();
			eventData.vlpTargets[i]->OnClick(eventData);
			eventData.lpLastTarget = eventData.vlpTargets[i];
			if (eventData.vlpTargets[i]->IsCanPassEvent()) eventData.Reset();
			else break;
		}
	}

	// OnEndDrag & OnDrop
	if (eventData.message[(int)EVENT_TYPE::END_DRAG] && eventData.message[(int)EVENT_TYPE::DROP])
	{
		if (eventData.isDragging)
		{
			// OnEndDrag
			eventData.type = EVENT_TYPE::END_DRAG;
			eventData.Use();
			eventData.lpLastTarget = eventData.lpDragTarget;
			eventData.lpDragTarget->OnEndDrag(eventData);
	
			// OnDrop
			eventData.type = EVENT_TYPE::DROP;
			for (int i = 0; i < eventData.vlpTargets.size(); ++i)
			{
				eventData.Use();
				eventData.vlpTargets[i]->OnDrop(eventData);
				eventData.lpLastTarget = eventData.vlpTargets[i];
				if (eventData.vlpTargets[i]->IsCanPassEvent()) eventData.Reset();
				else break;
			}
			eventData.Use();
			eventData.isDragging = false;
			eventData.lpDragTarget = nullptr;
		}
	}
	
	// OnDrag || OnBeginDrag
	if (eventData.message[(int)EVENT_TYPE::DRAG] || eventData.message[(int)EVENT_TYPE::BEGIN_DRAG])
	{
		if (eventData.isDragging)
		{
			// OnDrag
			eventData.type = EVENT_TYPE::DRAG;
			eventData.Use();
			eventData.lpDragTarget->OnDrag(eventData);
		}
		else if (!eventData.vlpTargets.empty())
		{
			// OnBeginDrag
			eventData.type = EVENT_TYPE::BEGIN_DRAG;
			eventData.dragPoint = eventData.point;
			for (int i = 0; i < eventData.vlpTargets.size(); ++i)
			{
				eventData.Use();
				eventData.isDragging = true;
				eventData.lpDragTarget = eventData.vlpTargets[i];
				eventData.lpDragTarget->OnBeginDrag(eventData);
				if (eventData.vlpTargets[i]->IsCanPassEvent()) eventData.Reset();
				else break;
			}
		}
	}
}

HRESULT EventSystem::Init()
{
	eventData.vlpTargets.reserve(10);
	return S_OK;
}

void EventSystem::Release()
{
}

void EventSystem::Update(float deltaTime)
{
	// EventData 초기화
	InitEventData(eventData);
	eventData.deltaTime = deltaTime;
	eventData.point = KeyManager::GetSingleton()->GetMousePoint();

	// 내 마우스 위치에서 최상위에 있는 UI를 Get
	FindDispatcherUI(vFindUIs, lpGameUI, eventData.point);
	eventData.vlpTargets.insert(eventData.vlpTargets.end(), vFindUIs.crbegin(), vFindUIs.crend());

	// 내 마우스 위치에서 최상위에 있는 Object를 Get
	FindDispatcherObject(vFindObjects, lpGameObject, eventData.point);
	eventData.vlpTargets.insert(eventData.vlpTargets.end(), vFindObjects.crbegin(), vFindObjects.crend());

	// 이벤트 처리
	EventProcess();
}

void EventSystem::Render(HDC hdc)
{
	SetBkMode(hdc, OPAQUE);
	if (!eventData.vlpTargets.empty())
	{
		int lineHeight = 10;
		string text = to_string(eventData.point.x) + ", " + to_string(eventData.point.y) + " : " + to_string(eventData.deltaTime);
		TextOut(hdc, 300, lineHeight, text.c_str(), text.length());
		lineHeight += 15;
		for (int i = 0; i < eventData.vlpTargets.size(); ++i, lineHeight += 15)
		{
			text = string(typeid(*(eventData.vlpTargets[i])).name()) + ", " + to_string(eventData.vlpTargets[i]->IsCanPassEvent());
			TextOut(hdc, 300, lineHeight, text.c_str(), text.length());
		}
		text = eventData.message.to_string();
		TextOut(hdc, 300, lineHeight, text.c_str(), text.length());
	}
	SetBkMode(hdc, TRANSPARENT);
}

