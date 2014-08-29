// Back.cpp : 

#include "stdafx.h"

// CBackground class

CBackground::CBackground(void)
{
	// CBackground construction
}

CBackground::~CBackground(void)
{
	// CBackground destruction
}

bool CBackground::Create()
{
	tiles[0].file = IDB_SKY01;
	tiles[1].file = IDB_SKY02;

	for(int i=0 ; i<2 ; i++)
	{
		tiles[i].surf.width = TILE_WIDTH;
		tiles[i].surf.height = TILE_HEIGHT; 

		// create tile
		if(!tiles[i].surf.Create(g_pDD, TILE_WIDTH, TILE_HEIGHT))
			return false;

		if(!tiles[i].surf.LoadBitmap(g_hInst, tiles[i].file, 0, 0, TILE_WIDTH, TILE_HEIGHT))
			return false;
	}

	return true;
}

/*bool CBackground::Render()
{
	int map[4][12] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	};

	int width = TILE_WIDTH;
	int sourcex;

	int destx = 0;
	int desty;

	for(int i=(src_posx/TILE_WIDTH) ; i<(src_posx/TILE_WIDTH)+7 ; i++, destx+=TILE_WIDTH - (src_posx % TILE_WIDTH) )
	{
		// adjust for clipped tile at right side
		if( (destx + TILE_WIDTH) > SCREEN_WIDTH )
			width = SCREEN_WIDTH - destx;

		// adjust for clipped tile at left side
		if(i=(src_posx/TILE_WIDTH) && (src_posx % TILE_WIDTH) != 0)
		{
			sourcex = src_posx % TILE_WIDTH;
			width = TILE_WIDTH - (src_posx % TILE_WIDTH);
		}
		else
		{
			sourcex = 0;
		}

		for(int j=0, desty=0; j<4 ; j++, desty+=TILE_HEIGHT)
		{
			tiles[map[j][i]].surf.Draw(g_pDDSBack, destx, desty, sourcex, 0, width, TILE_HEIGHT);
		
			char buf[200];
			sprintf(buf, "[%d, %d] sourcex = %d", j, i, sourcex);
			DrawGDIText(g_pDDSBack, buf, destx, desty, RGB(0,0,0));	
		}
	}

	return true;
}*/

bool CBackground::Render()
{
	int map[4][6] = {
	{1, 1, 1, 1, 1, 1},
	{1, 0, 1, 0, 1, 0},
	{1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1},
	};

	int width = TILE_WIDTH;

	int destx, desty;

	for(int i=0, destx=0 ; i<(SCREEN_WIDTH/TILE_WIDTH+1) ; i++, destx+=TILE_WIDTH)
	{
		// adjust for clipped tile
		if( (destx + TILE_WIDTH) > SCREEN_WIDTH )
			width = SCREEN_WIDTH - destx;

		for(int j=0, desty=0; j<4 ; j++, desty+=TILE_HEIGHT)
		{
			tiles[map[j][i]].surf.Draw(g_pDDSBack, destx, desty, 0 , 0, width, TILE_HEIGHT);
		}
	}

	return true;
}

int CBackground::GetTotalWidth()
{
	// return total surface width
	return -1;
}

bool CBackground::DrawGDIText(LPDIRECTDRAWSURFACE7 lpdds, char *text, int x,int y, COLORREF color)
{
	// render text at a given screen position
	HDC hdc;

	// get the dc from surface
	if (FAILED(lpdds->GetDC(&hdc)))
		return false;

	// create font
	HFONT hFont = CreateFont(9,						// height
							0,						// average character width
							0,						// angle of escapement
							0,						// baseline orientation angle
                            FW_NORMAL,				// font weight
							FALSE,					// italic attribute option
							FALSE,					// underline attribute option
							FALSE,					// strikeout attribute option
                            ANSI_CHARSET,			// character set identifier
							OUT_CHARACTER_PRECIS,	// output precision
                            CLIP_CHARACTER_PRECIS,	// clipping precision
							DEFAULT_QUALITY,		// output quality
                            DEFAULT_PITCH,			// pitch and family
							"Lucida Console");		// typeface name

	// select color and font
	SetTextColor(hdc, color);
	SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);

	// draw the text
	TextOut(hdc, x, y, text, strlen(text));

	// release the dc
	lpdds->ReleaseDC(hdc);

	// return success
	return true;
}