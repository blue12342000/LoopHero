#pragma once
#include "EventSystem.h"
#include "IClickHandler.h"
#include "IDragHandler.h"
#include "IDropHandler.h"
#include "IHoverHandler.h"
#include <Windows.h>

enum class EVENT_CATCH
{
	BLOCK,
	BLOCK_PASS,
	PASS
};

class EventTrigger : public IClickHandler, public IDragHandler, public IDropHandler, public IHoverHandler
{
private:
	EVENT_CATCH eventCatch;

public:
	EventTrigger():eventCatch(EVENT_CATCH::BLOCK) {}
	virtual ~EventTrigger() {}

	virtual void OnClick(EventData& data) override { data.Reset(); }
	virtual void OnBeginDrag(EventData& data) override { data.Reset(); }
	virtual void OnDrag(EventData& data) override { data.Reset(); }
	virtual void OnEndDrag(EventData& data) override { data.Reset(); }
	virtual void OnDrop(EventData& data) override { data.Reset(); }
	virtual void OnMouseEnter(EventData& data) override { data.Reset(); };
	virtual void OnMouseOver(EventData& data) override { data.Reset(); };
	virtual void OnMouseOut(EventData& data) override { data.Reset(); };

	inline bool IsCanCatchEvent() { return eventCatch != EVENT_CATCH::PASS; }
	inline bool IsCanPassEvent() { return eventCatch != EVENT_CATCH::BLOCK; }
	inline bool IsAllCatchEvent() { return eventCatch == EVENT_CATCH::BLOCK; }
	inline void SetEventCatch(EVENT_CATCH eventCatch) { this->eventCatch = eventCatch; }
};

