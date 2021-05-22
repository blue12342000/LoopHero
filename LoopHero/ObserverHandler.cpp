#include "ObserverHandler.h"

void ObserverHandler::AddOEventHandler(string message, function<void(ObserverHandler*)> func)
{
	mOEvents.insert(make_pair(message, func));
}

void ObserverHandler::RemoveOEventHandler(string message)
{
	mOEvents.erase(message);
}

void ObserverHandler::Notify(string message, ObserverHandler* lpCaller)
{
	if (mOEvents.find(message) != mOEvents.end())
	{
		mOEvents[message](lpCaller);
	}
}
