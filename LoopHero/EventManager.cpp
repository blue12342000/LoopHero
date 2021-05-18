#include "EventManager.h"
#include "EventHandler.h"

void EventManager::RegisterObserver(EventHandler* observe)
{
	sObserver.insert(observe);
}

void EventManager::DeleteObserver(EventHandler* observe)
{
	sObserver.erase(observe);
}

void EventManager::Notify(string eventKey, EventHandler& caller)
{
	for (const EventHandler* lpObserve : sObserver)
	{
		lpObserve->Notify(eventKey, caller);
	}
}
