#include "EventHandler.h"
#include "EventManager.h"
#include "GameEvent.h"

EventHandler::EventHandler()
{
	EventManager::GetSingleton()->RegisterObserver(this);
}

EventHandler::~EventHandler()
{
	EventManager::GetSingleton()->DeleteObserver(this);
}

void EventHandler::AddGameEvent(string eventKey, GameEvent* lpGameEvent)
{
	mEventFunc.insert(make_pair(eventKey, move(lpGameEvent)));
}

void EventHandler::RemoveGameEvent(string eventKey)
{
	mEventFunc.erase(eventKey);
}

void EventHandler::Notify(string eventKey, EventHandler& caller) const
{
	const auto it = mEventFunc.find(eventKey);
	if (it == mEventFunc.end()) return;
	it->second->Exec(caller);
}
