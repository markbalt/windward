// Map.h :

#pragma once

class CMap : public CLevel
{
public:
	CMap();
	~CMap();
	
	// members
	bool Create();
	bool Render();
	int GetTotalWidth();

private:
	// map surfaces
	CSurface lev1_map;
};