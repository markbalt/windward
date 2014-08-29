// Player.h :

#pragma once

// CPlayer class

class CPlayer : public CSprite
{
public:
	CPlayer(void);
	~CPlayer(void);

// members
	bool Create();
	bool Render();

	int CurrentWidth();
	int CurrentHeight();

	// jump time = time since beginning of jump
	DWORD jump_time;

	// tick = current time
	DWORD tick;

private:

// surfaces
	CSurface cloud_idle, cloud_run, cloud_prone, cloud_jump, cloud_fall;

	// render state functions
	bool render_idle();
	bool render_run();	
	bool render_prone();
	bool render_jump();
	bool render_fall();
};