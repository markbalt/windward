// Map.cpp : 

#include "stdafx.h"

// CMap class

CMap::CMap(void)
{
	// CMap construction
}

CMap::~CMap(void)
{
	// CMap destruction
}

bool CMap::Create()
{
	// init level attributes
	lev1_map.width = SCREEN_WIDTH;
	lev1_map.height = SCREEN_HEIGHT;

	// create level surface and load bitmap
	if(!lev1_map.Create(g_pDD, 2048, 768, COLOR_KEY))
		return false;

	if(!lev1_map.LoadBitmap(g_hInst, IDB_LEVEL1_MAP, 0, 0, 2048, 768))
		return false;

	return true;
}

bool CMap::Render()
{
	// render level map
	return lev1_map.Draw(g_pDDSBack, 0, 0, src_posx , src_posy, 1024, 768);
}

int CMap::GetTotalWidth()
{
	// return total surface width
	return lev1_map.surfwidth;
}