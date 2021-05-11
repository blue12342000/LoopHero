#pragma once

#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <map>
#include <ctime>
#include <random>

using namespace std;

#define WINPOS_STARTX 100
#define WINPOS_STARTY 100
#define WINSIZE_WIDTH 1288
#define WINSIZE_HEIGHT 728

#define PI 3.141592f

extern HINSTANCE g_hInstance;
extern HWND g_hWnd;
extern bool isDebugMode;

#include "KeyManager.h"
#include "ImageManager.h"
#include "SceneManager.h"
#include "DataManager.h"