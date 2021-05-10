#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <vector>

using namespace std;

class GameObject;
class RendererManager
{
private:
	vector<GameObject*> vLpGameObjects;

public:
	HRESULT Init();
	void Release();
	void Render(HDC hdc);
};

