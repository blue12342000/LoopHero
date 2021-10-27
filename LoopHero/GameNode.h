#pragma once
#include "ObserverHandler.h"
#include <iostream>
#include <string>

using namespace std;

class GameNode : public ObserverHandler
{
public:
	virtual ~GameNode() {}

	virtual string ToString()
	{
		return typeid(*this).name();
	}
};

