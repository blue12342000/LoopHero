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
#include <random>

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
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#define malloc(s) _malloc_dbg(s,_NORMAL_BLOCK,__FILE__,__LINE__)
#endif