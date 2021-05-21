#include "ObserverManager.h"
#include "ObserverHandler.h"

void ObserverManager::Release()
{
	sObservers.clear();
}

void ObserverManager::RegisterObserver(ObserverHandler* lpObserver)
{
	sObservers.insert(lpObserver);
}

void ObserverManager::RemoveObserver(ObserverHandler* lpObserver)
{
	sObservers.erase(lpObserver);
}

void ObserverManager::Notify(string message, ObserverHandler& caller)
{
	for (auto& lpObserver : sObservers)
	{
		if (lpObserver == &caller) continue;

		lpObserver->Notify(message, caller);
	}
}
