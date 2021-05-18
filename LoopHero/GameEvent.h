#pragma once

class EventHandler;
class GameEvent
{
public:
	void Exec(EventHandler& caller);
};

