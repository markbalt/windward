// AppMain.cpp : defines the entry point for the application
// includes WimMain, message/game loop, and message handler

#include "stdafx.h"

// globals

HWND        g_hMainWnd;							// handle of the main window
HINSTANCE	g_hInst;							// instance handle of the application
CGameEngine	gameEngine;							// new game engine instance

LPDIRECTDRAW7			g_pDD = NULL;			// directDraw object
LPDIRECTDRAWSURFACE7	g_pDDSFront = NULL;		// directDraw frontbuffer surface
LPDIRECTDRAWSURFACE7	g_pDDSBack = NULL;		// directDraw backbuffer surface
LPDIRECTINPUT			lpDI;					// directInput object
LPDIRECTINPUTDEVICE		lpDIKey;				// directInput keyboard device

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// initialize and create our window
	g_hInst = hInstance;
	g_hMainWnd = InitWindow(nCmdShow);

	// check return value of window creation
	if(!g_hMainWnd)
		return -1;

	// initialize Direct Draw
	if(InitDirectDraw() < 0)
	{
		CleanUp();
		MessageBox(g_hMainWnd, "error initializing DirectDraw, please update DirectX", "error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	// initialize Direct Input
	if(InitDirectInput() < 0)
	{
		CleanUp();
		MessageBox(g_hMainWnd, "error initializing DirectInput, please update DirectX", "error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	if(!gameEngine.Create())
	{
		CleanUp();
		MessageBox(g_hMainWnd, "error creating game engine class", "error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	// message/game loop
	while( TRUE )
	{
		MSG msg;

		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			// check for a quit message
			if( msg.message == WM_QUIT )
				break;

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			ProcessIdle();
		}
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// catch destroy or escape messages
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
			return 0;
		}
		break;

	} // message switch

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HWND InitWindow(int iCmdShow)
{
	HWND      hWnd;
	WNDCLASS  wc;

	// register a window class in the windows environment (needed to create a window)
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_hInst;
	wc.hIcon = LoadIcon(g_hInst, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = TEXT("");
	wc.lpszClassName = TEXT(CLASS_NAME);
	RegisterClass(&wc);

	// create a full screen window
	hWnd = CreateWindowEx(WS_EX_TOPMOST,
						  TEXT(CLASS_NAME),					// class
						  TEXT(CAPTION),					// caption
						  WS_POPUP,							// style
						  0,								// left
						  0,								// top
						  GetSystemMetrics(SM_CXSCREEN),	// width
						  GetSystemMetrics(SM_CYSCREEN),	// height
						  NULL,								// parent window
						  NULL,								// menu
						  g_hInst,							// instance
						  NULL);							// params

	// show window and cursor
	ShowWindow(hWnd, iCmdShow);
	ShowCursor(true);
	UpdateWindow(hWnd);
	SetFocus(hWnd);

	// returns a handle to our newly created window
	return hWnd;
}

void ProcessIdle()
{
	// process graphics during idle
	gameEngine.ProcessIdle();
}

int InitDirectDraw()
{
	DDSURFACEDESC2 ddsd;
	DDSCAPS2 ddscaps;
	HRESULT hRet;

	// create the main DirectDraw object.
	hRet = DirectDrawCreateEx(NULL, (VOID**)&g_pDD, IID_IDirectDraw7, NULL);
	if( hRet != DD_OK )
		return -1;

	// get exclusive mode.
	hRet = g_pDD->SetCooperativeLevel(g_hMainWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
	if( hRet != DD_OK )
		return -2;

	// set the video mode to 1024x768x32.
	hRet = g_pDD->SetDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0);
	if( hRet != DD_OK )
		return -3;

	// prepare to create the primary surface by initializing the fields of a DDSURFACEDESC2 structure.
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP |DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = 1;

	// create the primary surface.
	hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSFront, NULL);
	if( hRet != DD_OK )
		return -1;

	// get a pointer to the back buffer.
	ZeroMemory(&ddscaps, sizeof(ddscaps));
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	hRet = g_pDDSFront->GetAttachedSurface(&ddscaps, &g_pDDSBack);
	if( hRet != DD_OK )
		return -1;

	return 0;
}

int InitDirectInput()
{
	HRESULT hRet;

	// direct Input
	hRet = DirectInput8Create(g_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8A, (VOID**)&lpDI, NULL);
	if (hRet!=DI_OK)
		return -1;

	// create Keyboard Device
	hRet = lpDI->CreateDevice(GUID_SysKeyboard, &lpDIKey, NULL);
	if (hRet!=DI_OK)
		return -2;

	// set co-op
	hRet = lpDIKey->SetCooperativeLevel(g_hMainWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (hRet!=DI_OK)
		return -3;

	// set data format
	hRet = lpDIKey->SetDataFormat(&c_dfDIKeyboard);
	if (hRet!=DI_OK)
		return -4;
	
	// acquire data stream from Device
	hRet = lpDIKey->Acquire();
	if (hRet!=DI_OK)
		return -5;

	return 0;
}

BOOL PollKeyDown(unsigned int ibutton)
{
	// check requested keystate
	UCHAR keystate[256];
	lpDIKey->GetDeviceState(256, keystate);
	if (keystate[ibutton] & 0x80)
		return true;
	else
		return false;
}

BOOL PollKeyUp(unsigned int ibutton)
{
	// check requested keystate
	UCHAR keystate[256];
	lpDIKey->GetDeviceState(256, keystate);
	if (!(keystate[ibutton] & 0x80))
		return true;
	else
		return false;
}

void CleanUp()
{
	// back Buffer
	if(g_pDDSBack)
		g_pDDSBack->Release();

	// front Buffer
	if(g_pDDSFront)
		g_pDDSFront->Release();

	// ddraw object
	if(g_pDD)
		g_pDD->Release();

	// dinput object
	if(lpDI)
		lpDI->Release();

	// keyboard device
	if(lpDIKey)
		lpDIKey->Release();
}