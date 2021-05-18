#pragma once
#include <map>
#include <string>
#include <functional>

using namespace std;

class GameEvent;
class EventHandler
{
private:
	map<string, GameEvent*> mEventFunc;

public:
	EventHandler();
	virtual ~EventHandler();

	void AddGameEvent(string eventKey, GameEvent* lpGameEvent);
	void RemoveGameEvent(string eventKey);
	void Notify(string eventKey, EventHandler& caller) const;
};

