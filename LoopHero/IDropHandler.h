#pragma once

struct EventData;
class IDropHandler
{
public:
	virtual void OnDrop(EventData& data) = 0;
};

