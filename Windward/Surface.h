// Surface.h : defines base class for manipulation and blitting of surfaces

class CSurface
{
public:
	LPDIRECTDRAWSURFACE7 GetSurface();

	CSurface();
	virtual ~CSurface();
	void Restore();
	void Destroy();
	BOOL Create(LPDIRECTDRAW7 hDD, int width, int height, COLORREF dwColorKey = -1);
	BOOL LoadBitmap(HINSTANCE hInst, UINT nRes, int x = 0, int y = 0, int width = 0, int height = 0);
	BOOL Draw(LPDIRECTDRAWSURFACE7 lpDest, int destx = 0, int desty = 0, int srcx = 0, int srcy = 0, int width = 0, int height = 0, bool mirror = false);

	// surface attributes
	int surfwidth, surfheight;

	// width and height of each frame of bitmap
	int width, height;

protected:
	COLORREF m_ColorKey;
	LPDIRECTDRAWSURFACE7 m_surface;
};