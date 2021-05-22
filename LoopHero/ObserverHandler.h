#pragma once
#include <map>
#include <string>
#include <functional>

using namespace std;

class ObserverHandler
{
private:
	map<string, function<void(ObserverHandler*)>> mOEvents;

public:
	virtual ~ObserverHandler() {}

	void AddOEventHandler(string message, function<void(ObserverHandler*)>);
	void RemoveOEventHandler(string message);

	void Notify(string message, ObserverHandler* lpCaller);
};

