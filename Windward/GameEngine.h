// Pawn.h : defines single player game loop


// external access to global direct draw members

extern HWND g_Hwnd;							// handle of the main window
extern HINSTANCE g_hInst;					// instance handle of our application
extern LPDIRECTDRAW7 g_pDD;					// DirectDraw object
extern LPDIRECTDRAWSURFACE7 g_pDDSFront;	// DirectDraw fronbuffer surface
extern LPDIRECTDRAWSURFACE7 g_pDDSBack;		// DirectDraw backbuffer surface

class CGameEngine
{
public:
	CGameEngine(void);
	~CGameEngine(void);

	bool Create();
	void ProcessIdle();

protected:
	void ReactToInput();
	void UpdateScrolling();
	void FlipBuffer();
	void ShowLoadScreen(char *text);

	bool DrawGDIText(LPDIRECTDRAWSURFACE7 lpdds, char *text, int x,int y, COLORREF color);

	// member variables
	bool debug;
	HRESULT hRet;

	// time syncs
	DWORD lastblit;

	// for parallelax scrolling
	bool scroll;
	int sprite_speed, bg_speed;
	bool space_release;

	// color key for transparency
	COLORREF color_key;

	// sprites
	CPlayer m_cloud;

	// level map and background
	CMap m_map;
	CBackground m_back;
};