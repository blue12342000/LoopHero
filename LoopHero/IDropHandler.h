#pragma once

struct EventData;
interface IDropHandler
{
public:
	virtual void OnDrop(EventData& data) = 0;
};

