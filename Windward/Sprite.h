// Sprite.h : defines classe for sprite surface characteristics and rendering

#pragma once

// CSprite class

enum sprite_state{idle, idle_wait, run_left, run_right, jump, fall, prone};

class CSprite
{
public:
	CSprite(void);
	~CSprite(void);

	sprite_state cur_state;
	sprite_state last_state;

	// position on screen ; position in level
	int screenx, screeny;
	int mapx, mapy;

	// current source position for dividing multiple sprites on each bitmap
	int src_posx, src_posy;
};
