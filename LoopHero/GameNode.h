#pragma once
#include <iostream>
#include <string>

using namespace std;

class GameNode
{
public:
	virtual string ToString()
	{
		return typeid(*this).name();
	}
};

