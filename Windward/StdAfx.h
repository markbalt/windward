// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

// standard includes and initializations

#pragma once

#define WIN32_LEAN_AND_MEAN		// exclude rarely-used stuff from Windows headers

#include <windows.h>

// all headers the application requires go here

#include <ddraw.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <stdio.h>
#include "AppMain.h"
#include "Surface.h"
#include "Sprite.h"
#include "Player.h"
#include "Enemy.h"
#include "Element.h"
#include "Level.h"
#include "Map.h"
#include "Tile.h"
#include "Background.h"
#include "GameEngine.h"
#include "resource.h"