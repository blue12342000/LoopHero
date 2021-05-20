#pragma once

struct EventData;
class IClickHandler
{
public:
	virtual void OnClick(EventData& data) = 0;
};

