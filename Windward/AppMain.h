// AppMain.h : defines main application globals

#pragma once

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

#define COLOR_KEY RGB(255, 0, 255)
#define CAPTION "Windward"
#define	CLASS_NAME "dd_windward"

HWND InitWindow(int iCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void ProcessIdle();
int InitDirectDraw();
int InitDirectInput();
BOOL PollKeyDown(unsigned int ibutton);
BOOL PollKeyUp(unsigned int ibutton);
void CleanUp();