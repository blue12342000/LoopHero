#pragma once

#include <iostream>
#include <Windows.h>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <ctime>
#include <random>
#include <functional>

using namespace std;

#define WINPOS_STARTX 100
#define WINPOS_STARTY 100
#define WINSIZE_WIDTH 1280
#define WINSIZE_HEIGHT 720

#define PI 3.141592f

extern HINSTANCE g_hInstance;
extern HWND g_hWnd;
extern bool isDebugMode;

#include "KeyManager.h"
#include "ImageManager.h"
#include "SceneManager.h"
#include "DataManager.h"
#include "GameData.h"
#include "ObserverManager.h"
#include "PoolingManager.h"
#include "FontManager.h"
#include "ParticleManager.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif