#pragma once

class GameNode
{
public:
	virtual string ToString()
	{
		return typeid(*this).name();
	}
};

