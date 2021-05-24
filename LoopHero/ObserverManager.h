#pragma once
#include "Singleton.h"
#include <set>
#include <string>
#include <list>

using namespace std;

class ObserverHandler;
struct NotifyMessage
{
	string message;
	ObserverHandler* lpCaller;
};

class ObserverManager : public Singleton<ObserverManager>
{
private:
	set<ObserverHandler*> sObservers;
	list<NotifyMessage> lMessages;

public:
	void Release();
	void ProcessingMessage();

	void RegisterObserver(ObserverHandler* lpObserver);
	void RemoveObserver(ObserverHandler* lpObserver);
	void Notify(string message, ObserverHandler* lpCaller);
};

