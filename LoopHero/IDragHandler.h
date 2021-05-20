#pragma once

struct EventData;
class IDragHandler
{
public:
	virtual void OnBeginDrag(EventData& data) = 0;
	virtual void OnDrag(EventData& data) = 0;
	virtual void OnEndDrag(EventData& data) = 0;
};

