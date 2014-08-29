// Player.cpp : 

#include "stdafx.h"

// CPlayer class

CPlayer::CPlayer(void)
{
	// CPlayer construction
	cur_state = idle;
	last_state = run_right;
}

CPlayer::~CPlayer(void)
{
	// CPlayer destruction
}

bool CPlayer::Create()
{
	// create all player surfaces and init characterisitcs

	// cloud idle
	cloud_idle.width = 36;
	cloud_idle.height = 40;
	cloud_idle.surfwidth = 36;
	cloud_idle.surfheight = 40;

	if(!cloud_idle.Create(g_pDD, 36, 40, COLOR_KEY))
		return false;
	
	if(!cloud_idle.LoadBitmap(g_hInst, IDB_CLOUD_IDLE, 0, 0, 36, 40))
		return false;

	// cloud run
	cloud_run.width = 50;
	cloud_run.height = 40;
	cloud_run.surfwidth = 750;
	cloud_run.surfheight = 40;
	
	if(!cloud_run.Create(g_pDD, 750, 40, COLOR_KEY))
		return false;

	if(!cloud_run.LoadBitmap(g_hInst, IDB_CLOUD_RUN, 0, 0, 750, 40))
		return false;

	// cloud prone
	cloud_prone.width = 40;
	cloud_prone.height = 36;
	cloud_prone.surfwidth = 40;
	cloud_prone.surfheight = 36;
	
	if(!cloud_prone.Create(g_pDD, 40, 36, COLOR_KEY))
		return false;

	if(!cloud_prone.LoadBitmap(g_hInst, IDB_CLOUD_PRONE, 0, 0, 40, 36))
		return false;

	// cloud jump
	cloud_jump.width = 47;
	cloud_jump.height = 54;
	cloud_jump.surfwidth = 188;
	cloud_jump.surfheight = 54;
	
	if(!cloud_jump.Create(g_pDD, 188, 54, COLOR_KEY))
		return false;

	if(!cloud_jump.LoadBitmap(g_hInst, IDB_CLOUD_JUMP, 0, 0, 188, 54))
		return false;

	// cloud fall
	cloud_fall.width = 47;
	cloud_fall.height = 54;
	cloud_fall.surfwidth = 94;
	cloud_fall.surfheight = 54;
	
	if(!cloud_fall.Create(g_pDD, 94, 54, COLOR_KEY))
		return false;

	if(!cloud_fall.LoadBitmap(g_hInst, IDB_CLOUD_FALL, 0, 0, 94, 54))
		return false;

	return true;
}

bool CPlayer::Render()
{
	// Draw(buffer surface, x screen pos, y screen pos, source x, source y, width, height, mirror?)

	// render sprite in current state
	switch(cur_state)
	{
	case idle: return render_idle(); break;
	case run_left: return render_run(); break;
	case run_right: return render_run(); break;
	case prone: return render_prone(); break;
	case jump: return render_jump(); break;
	case fall: return render_fall(); break;
	default: return false; break;
	}
}

bool CPlayer::render_idle()
{
	// render idle sprite
	src_posx = src_posy = 0;

	if(last_state == run_right)
		return cloud_idle.Draw(g_pDDSBack, screenx, screeny, src_posx, src_posy, cloud_idle.width, cloud_idle.height, false);
	if(last_state == run_left)
		return cloud_idle.Draw(g_pDDSBack, screenx, screeny, src_posx, src_posy, cloud_idle.width, cloud_idle.height, true);

	return false;
}

bool CPlayer::render_run()
{
	// render running sprite
	bool result = false;

	if(cur_state == run_right)
		result = cloud_run.Draw(g_pDDSBack, screenx, screeny, src_posx, src_posy, cloud_run.width, cloud_run.height, false);
	if(cur_state == run_left)
		result = cloud_run.Draw(g_pDDSBack, screenx, screeny, src_posx, src_posy, cloud_run.width, cloud_run.height, true);

	// update source position
	src_posx += cloud_run.width;

	if(src_posx >= cloud_run.surfwidth)
		src_posx = 0;

	return result;
}

bool CPlayer::render_prone()
{
	// render prone sprite
	src_posx = src_posy = 0;

	if(last_state == run_right)
		return cloud_prone.Draw(g_pDDSBack, screenx, screeny, src_posx, src_posy, cloud_prone.width, cloud_prone.height, false);
	if(last_state == run_left)
		return cloud_prone.Draw(g_pDDSBack, screenx, screeny, src_posx, src_posy, cloud_prone.width, cloud_prone.height, true);

	return false;
}

bool CPlayer::render_jump()
{
	// render jump sprite
	src_posx = src_posy = 0;

	// choose frame in jump sequence
	if (tick - jump_time < 60)
	{ 
		src_posx = 0;
	}
	else if ((tick - jump_time >=60) && (tick - jump_time < 120)) 
	{
		src_posx = cloud_jump.width;
	}
	else if ((tick - jump_time >=120) && (tick - jump_time < 160))
	{
		src_posx = cloud_jump.width * 2;
	}
	else if ((tick - jump_time >=160) && (tick - jump_time < 200))
	{
		src_posx = cloud_jump.width * 2;
	}

	// draw this frame
	if(last_state == run_right)
		return cloud_jump.Draw(g_pDDSBack, screenx, screeny, src_posx, src_posy, cloud_jump.width, cloud_jump.height, false);
	if(last_state == run_left)
		return cloud_jump.Draw(g_pDDSBack, screenx, screeny, src_posx, src_posy, cloud_jump.width, cloud_jump.height, true);

	return false;
}

bool CPlayer::render_fall()
{
	// render fall sprite
	bool result = false;

	if(last_state == run_right)
		result = cloud_fall.Draw(g_pDDSBack, screenx, screeny, src_posx, src_posy, cloud_fall.width, cloud_fall.height, false);
	if(last_state == run_left)
		result = cloud_fall.Draw(g_pDDSBack, screenx, screeny, src_posx, src_posy, cloud_fall.width, cloud_fall.height, true);

	// update source position
	src_posx += cloud_fall.width;

	if(src_posx >= cloud_fall.surfwidth)
		src_posx = cloud_fall.surfwidth - cloud_fall.width;

	return result;
}

int CPlayer::CurrentWidth()
{
	// return width of current sprite surface frame
	switch(cur_state)
	{
	case idle: return cloud_idle.width; break;
	case prone: return cloud_prone.width; break;
	case run_left: return cloud_run.width; break;
	case run_right: return cloud_run.width; break;
	case jump: return cloud_jump.width; break;
	case fall: return cloud_fall.width; break;
	}

	return -1;
}

int CPlayer::CurrentHeight()
{
	// return height of current sprite surface frame
	switch(cur_state)
	{
	case idle: return cloud_idle.height; break;
	case prone: return cloud_prone.height; break;
	case run_left: return cloud_run.height; break;
	case run_right: return cloud_run.height; break;
	case jump: return cloud_jump.height; break;
	case fall: return cloud_fall.height; break;
	}

	return -1;
}