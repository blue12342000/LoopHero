#include "KeyManager.h"

HRESULT KeyManager::Init()
{
    isKeyUp.set();
    isKeyDown.reset();
    return S_OK;
}

bool KeyManager::IsKeyDownOne(int input)
{
    if (GetAsyncKeyState(input) & 0x8000)
    {
        if (!isKeyDown[input])
        {
            isKeyDown[input] = true;
            return true;
        }
    }
    else
    {
        isKeyDown[input] = false;
    }
    return false;
}

bool KeyManager::IsKeyUpOne(int input)
{
    if (GetAsyncKeyState(input) & 0x8000)
    {
        isKeyUp[input] = false;
    }
    else
    {
        if (!isKeyUp[input])
        {
            isKeyUp[input] = true;
            return true;
        }
    }
    return false;
}

bool KeyManager::IsKeyDownStay(int input)
{
    if (GetAsyncKeyState(input) & 0x8000)
    {
        return true;
    }
    return false;
}
