#pragma once

struct EventData;
class IHoverHandler
{
public:
	virtual void OnMouseEnter(EventData& data) = 0;
	virtual void OnMouseOver(EventData& data) = 0;
	virtual void OnMouseOut(EventData& data) = 0;
};

