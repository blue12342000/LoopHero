#pragma once
#include "IClickHandler.h"
#include "IDragHandler.h"
#include "IDropHandler.h"
#include <Windows.h>

enum class EVENT_TYPE
{
	CLICK,
	BEGIN_DRAG,
	DRAG,
	END_DRAG,
	NONE
};

class EventTrigger;
struct EventData
{
	bool isUsed;
	float deltaTime;
	EVENT_TYPE type;
	POINT point;
	EventTrigger* lpTarget;
};

class EventTrigger : public IClickHandler, IDragHandler, IDropHandler
{
public:
	virtual void OnClick(EventData& data) override { data.isUsed = false; }
	virtual void OnBeginDrag(EventData& data) override { data.isUsed = false; }
	virtual void OnDrag(EventData& data) override { data.isUsed = false; }
	virtual void OnEndDrag(EventData& data) override { }
	virtual void OnDrop(EventData& data) override { data.isUsed = false; }
};

