// Level.h : defines classe for level surface characteristics and rendering

#pragma once

class CLevel
{
public:
	CLevel();
	~CLevel();

	// position on screen
	int screenx, screeny;

	// current source position
	int src_posx, src_posy;
};
