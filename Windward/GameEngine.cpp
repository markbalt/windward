// Pawn.cpp : single player game loop

#include "stdafx.h"

CGameEngine::CGameEngine(void)
{
	// CGameEngine construction
	debug = false;
	scroll = false;
	space_release = true;

	lastblit = 0;
	sprite_speed = 16;
	bg_speed = 4;
}

CGameEngine::~CGameEngine(void)
{
	// CGameEngine destruction
}

bool CGameEngine::Create()
{
	// initialize new instance of pawn class

	// create level
	ShowLoadScreen("Loading level background");
	if(!m_back.Create())
	{
		ShowLoadScreen("Level background creation failed");
		return false;
	}

	ShowLoadScreen("Loading level map");
	if(!m_map.Create())
	{
		ShowLoadScreen("Level map creation failed");
		return false;
	}

	// create sprites
	ShowLoadScreen("Loading sprite");

	// init sprite position and state
	m_cloud.mapx = 200;
	m_cloud.mapy = 634;
	m_cloud.screenx = 200;
	m_cloud.screeny = 634;
	m_cloud.Create();
	m_cloud.cur_state = run_right;

	return true;
}

void CGameEngine::ProcessIdle()
{
	// CGameEngine::ProcessIdle = the game loop
	ShowCursor(false);

	// allow each frame to stay on the screen for at least 50 miliseconds
	if(GetTickCount() - lastblit < 50)
	{
		return;
	}

	// update sprites and elements dependent upon user input
	ReactToInput();

	// update scrolling
	UpdateScrolling();
	
	// render level
	m_back.Render();
	m_map.Render();

	// render sprites
	m_cloud.Render();

	// debug output
	char buf[100];
	sprintf_s(buf,"debug info");
	//DrawGDIText(g_pDDSBack, buf, 10, 25, RGB(0,0,0));

	// output framerate
	sprintf_s(buf,"%.0f fps", (double) 1000/(GetTickCount() - lastblit) );
	DrawGDIText(g_pDDSBack, buf, 10, 10, RGB(0,0,0));

	// flip buffers mark tick count for frame rate
	FlipBuffer();
	lastblit = GetTickCount();
}

void CGameEngine::ReactToInput()
{
	// poll keyboard for user input and update sprites and elements dependent on such
	// known issues: when moving right and holding the up arrow, jumping does not work

	// space bar release
	if(PollKeyUp(DIK_SPACE))
	{
		space_release = true;
	}

	// release space bar while jumping = fall
	if(m_cloud.cur_state == jump && PollKeyUp(DIK_SPACE))
	{
		m_cloud.cur_state = fall;
		m_cloud.src_posx = m_cloud.src_posy = 0;
	}

	// check for fall state
	if(m_cloud.cur_state == fall)
	{
		m_cloud.screeny += 20;
		m_cloud.mapy += 20;

		// temporary ; keep sprite on map
		if(m_cloud.mapy > 634)
		{
			m_cloud.screeny = 634;
			m_cloud.mapy = 634;
			m_cloud.src_posx = m_cloud.src_posy = 0;
			m_cloud.cur_state = idle;
		}
	}

	// check down key down = prone
	if(PollKeyDown(DIK_DOWN) && m_cloud.cur_state != prone && m_cloud.cur_state != fall && m_cloud.cur_state != jump)
	{
		// adjustments to sprite offset
		m_cloud.screeny += 3;
		m_cloud.mapy += 3;

		m_cloud.cur_state = prone;
	}

	// check down key up = prone over
	if(PollKeyUp(DIK_DOWN) && m_cloud.cur_state == prone && m_cloud.cur_state != fall)
	{
		// adjustments to sprite offset
		m_cloud.screeny -= 3;
		m_cloud.mapy -= 3;

		m_cloud.cur_state = idle;
	}

	// check down key down and another direction (prone direction switch)
	if(PollKeyDown(DIK_DOWN) && PollKeyDown(DIK_RIGHT) && m_cloud.cur_state != fall && m_cloud.cur_state != jump)
	{
		// verify direction
		m_cloud.last_state = run_right;
	}
	if(PollKeyDown(DIK_DOWN) && PollKeyDown(DIK_LEFT) && m_cloud.cur_state != fall && m_cloud.cur_state != jump)
	{
		// verify direction
		m_cloud.last_state = run_left;
	}

	// check right key down = moving right
	if(PollKeyDown(DIK_RIGHT) && !PollKeyDown(DIK_DOWN))
	{
		m_cloud.mapx += sprite_speed;
		m_cloud.last_state = run_right;

		if((m_cloud.cur_state != jump && m_cloud.cur_state != fall))
		{
			// set run state
			m_cloud.cur_state = run_right;
		}

		// update scroll or sprite location
		if(scroll)
		{
			m_back.src_posx += bg_speed;
			m_map.src_posx += sprite_speed;
		}
		else
		{
			m_cloud.screenx += sprite_speed;
		}
	}

	// check left key down = moving left
	if(PollKeyDown(DIK_LEFT) && !PollKeyDown(DIK_DOWN))
	{
		m_cloud.mapx -= sprite_speed;
		m_cloud.last_state = run_left;

		if(m_cloud.cur_state != jump && m_cloud.cur_state != fall)
		{
			// set run state
			m_cloud.cur_state = run_left;
		}

		// update scroll or sprite location
		if(scroll)
		{
			m_back.src_posx -= bg_speed;
			m_map.src_posx -= sprite_speed;
		}
		else
		{
			m_cloud.screenx -= sprite_speed;
		}
	}

	// check space bar down = jump
	if(PollKeyDown(DIK_SPACE) && m_cloud.cur_state != fall && m_cloud.cur_state != prone)//PollKeyUp(DIK_DOWN) )
	{
		// find jump squence timing		
		DWORD tick = GetTickCount();
		m_cloud.tick = tick;

		if(m_cloud.cur_state != jump)
		{
			// start jump
			m_cloud.jump_time = tick;			
		}

		// verify user cannot repeat jump without releasing key first
		if(space_release)
		{
			m_cloud.src_posx = m_cloud.src_posy = 0;
			m_cloud.cur_state = jump;
			space_release = false;
		}
		
		// choose position in jump sequence
		if(m_cloud.cur_state == jump)
		{
			if (tick - m_cloud.jump_time < 60)
			{
				m_cloud.screeny -= 40;
				m_cloud.mapy -= 40;
			}
			else if ((tick - m_cloud.jump_time >=60) && (tick - m_cloud.jump_time < 120)) 
			{
				m_cloud.screeny -= 25;
				m_cloud.mapy -= 25;
			}
			else if ((tick - m_cloud.jump_time >=120) && (tick - m_cloud.jump_time < 160))
			{
				m_cloud.screeny -= 20;
				m_cloud.mapy -= 20;
			}
			else if ((tick - m_cloud.jump_time >=160) && (tick - m_cloud.jump_time < 200)) 
			{
				m_cloud.screeny -= 15;
				m_cloud.mapy -= 15;
			}
			else
			{
				// currently falling
				m_cloud.cur_state = fall;
				m_cloud.src_posx = m_cloud.src_posy = 0;
			}
		}
	}

	// check no keys down = stand idle
	if(m_cloud.cur_state != fall && m_cloud.cur_state != jump && PollKeyUp(DIK_RIGHT) && PollKeyUp(DIK_LEFT) && PollKeyUp(DIK_DOWN))// && PollKeyUp(DIK_SPACE))
	{
		m_cloud.cur_state = idle;
	}
}

void CGameEngine::UpdateScrolling()
{
	// set scrolling value and update position if scrolling ; update sprites, level and background

	// avg player width while scrolling = 50 / 2
	int half_player = 25;

	// screen vs player metrics
	int middle = (SCREEN_WIDTH/2) - (half_player);
	int middle_end = m_map.GetTotalWidth() - (SCREEN_WIDTH/2) - (half_player);

	if(m_cloud.mapx >= middle && m_cloud.mapx <= middle_end)
	{
		// scrolling is active
		m_cloud.screenx = middle;
		scroll = true;
	}
	else
	{
		// scrolling is inactive ; beginning or end of level
		scroll = false;
		if(m_cloud.mapx < middle )
		{
			m_map.src_posx = 0;
			m_back.src_posx = 0;
		}
		if(m_cloud.mapx > middle_end )
		{
			m_map.src_posx = m_map.GetTotalWidth() - SCREEN_WIDTH;
		}
	}

	// bound the sprite into the level map
	if(m_cloud.mapx <= 0)
	{
		m_cloud.screenx = 0;
		m_cloud.mapx = 0;
	}
	if(m_cloud.mapx + m_cloud.CurrentWidth() >= m_map.GetTotalWidth())
	{
		m_cloud.screenx = SCREEN_WIDTH - m_cloud.CurrentWidth();
		m_cloud.mapx = m_map.GetTotalWidth() - m_cloud.CurrentWidth();
	}
}

void CGameEngine::FlipBuffer()
{
	// flip the newly rendered back buffer into the front buffer
	while(1)
	{
		hRet = g_pDDSFront->Flip(NULL, 0);
		if(hRet == DD_OK)
			break;
		if(hRet == DDERR_SURFACELOST)
			g_pDDSFront->Restore();
		if(hRet != DDERR_WASSTILLDRAWING)
			break;
	}
}

bool CGameEngine::DrawGDIText(LPDIRECTDRAWSURFACE7 lpdds, char *text, int x,int y, COLORREF color)
{
	// render text at a given screen position
	HDC hdc;

	// get the dc from surface
	if (FAILED(lpdds->GetDC(&hdc)))
		return false;

	// create font
	HFONT hFont = CreateFont(10,					// height
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

void CGameEngine::ShowLoadScreen(char *text)
{
	// show a loading screen
	HDC hdc;

	// get the dc from surface
	if (FAILED(g_pDDSBack->GetDC(&hdc)))
		return;

	// draw black screen
	RECT rect;
	SetRect(&rect, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	HBRUSH hbr = CreateSolidBrush(RGB(0,0,0));
	FillRect(hdc, &rect, hbr);

	// release back dc and draw text parameter
	g_pDDSBack->ReleaseDC(hdc);
	DrawGDIText(g_pDDSBack, text, 10, 50, RGB(0,255,255));
	FlipBuffer();
}