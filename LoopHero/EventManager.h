#pragma once
#include "Singleton.h"
#include <set>
#include <string>

using namespace std;

class EventHandler;
class EventManager : public Singleton<EventManager>
{
private:
	set<EventHandler*> sObserver;

public:
	void RegisterObserver(EventHandler* observe);
	void DeleteObserver(EventHandler* observe);

	void Notify(string eventKey, EventHandler& caller);
};

