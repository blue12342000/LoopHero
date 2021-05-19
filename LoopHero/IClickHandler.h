#pragma once

struct EventData;
interface IClickHandler
{
public:
	virtual void OnClick(EventData& data) = 0;
};

