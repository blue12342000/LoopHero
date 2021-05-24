#include "ObserverManager.h"
#include "ObserverHandler.h"

void ObserverManager::Release()
{
	sObservers.clear();
}

void ObserverManager::ProcessingMessage()
{
	while (!lMessages.empty())
	{
		for (const auto& lpObserver : sObservers)
		{
			if (lpObserver == lMessages.front().lpCaller) continue;
			lpObserver->Notify(lMessages.front().message, lMessages.front().lpCaller);
		}
		lMessages.pop_front();
	}
}

void ObserverManager::RegisterObserver(ObserverHandler* lpObserver)
{
	sObservers.insert(lpObserver);
}

void ObserverManager::RemoveObserver(ObserverHandler* lpObserver)
{
	sObservers.erase(lpObserver);
}

void ObserverManager::Notify(string message, ObserverHandler* lpCaller)
{
	lMessages.push_back({ message, lpCaller });
}
