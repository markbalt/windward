// Tile.h :

#pragma once

#define TILE_WIDTH	200
#define TILE_HEIGHT	192

class CTile
{
public:
	CTile(void);
	~CTile(void);

	// tile surfaces
	UINT file;
	CSurface surf;
};