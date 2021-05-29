#pragma once
#include <map>
#include <string>
#include <functional>

using namespace std;

class ObserverHandler
{
private:
	map<string, function<void(ObserverHandler*)>> mEvents;

public:
	virtual ~ObserverHandler() {}

	void AddEventHandler(string message, function<void(ObserverHandler*)>);
	void RemoveEventHandler(string message);

	inline void ClearEventHandler() { mEvents.clear(); }
	void Notify(string message, ObserverHandler* lpCaller);
};

