#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NOMINMAX
#include <windows.h>
#include <assert.h>
#include <functional>



#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>
#include <d3dx9.h>
#include <d3dx11.h>

#pragma warning(pop)



#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx11.lib")

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>  
#include <cereal/types/list.hpp>
#include "vector.h"

#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)
#define NOWVERSION		(0)

HWND GetWindow();

enum SceneState
{
	SCENESTATE_NONE = 0,
	SCENESTATE_SCENE,
	SCENESTATE_GAME,
};

enum GameState
{
	GAMESTATE_NONE = 0,
	GAMESTATE_STOP,
	GAMESTATE_PLAY,
};

using namespace std;