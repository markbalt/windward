// Back.h :

#pragma once

class CBackground : public CLevel
{
public:
	CBackground();
	~CBackground();

	// members
	bool Create();
	bool Render();
	int GetTotalWidth();

	CTile tiles[2];

	bool DrawGDIText(LPDIRECTDRAWSURFACE7 lpdds, char *text, int x, int y, COLORREF color);
};