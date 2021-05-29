#include "ObserverHandler.h"

void ObserverHandler::AddEventHandler(string message, function<void(ObserverHandler*)> func)
{
	mEvents.insert(make_pair(message, func));
}

void ObserverHandler::RemoveEventHandler(string message)
{
	mEvents.erase(message);
}

void ObserverHandler::Notify(string message, ObserverHandler* lpCaller)
{
	if (mEvents.find(message) != mEvents.end())
	{
		mEvents[message](lpCaller);
	}
}
