#pragma once
#include "Singleton.h"
#include <set>
#include <string>

using namespace std;

class ObserverHandler;
class ObserverManager : public Singleton<ObserverManager>
{
private:
	set<ObserverHandler*> sObservers;

public:
	void Release();

	void RegisterObserver(ObserverHandler* lpObserver);
	void RemoveObserver(ObserverHandler* lpObserver);
	void Notify(string message, ObserverHandler& caller);
};

