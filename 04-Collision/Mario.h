#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.1f 
//0.1f
#define MARIO_JUMP_SPEED_Y		0.1f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define SOPHIA_STATE_IDLE			0
#define SOPHIA_STATE_WALKING_RIGHT	100
#define SOPHIA_STATE_WALKING_LEFT	200
#define SOPHIA_STATE_JUMP			300
#define SOPHIA_STATE_DIE			400
#define SOPHIA_STATE_WALKING_DOWN	500

#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_SMALL_IDLE_RIGHT		2	
#define MARIO_ANI_SMALL_IDLE_LEFT			3

#define MARIO_ANI_BIG_WALKING_RIGHT			4
#define MARIO_ANI_BIG_WALKING_LEFT			5
#define MARIO_ANI_SMALL_WALKING_RIGHT		6
#define MARIO_ANI_SMALL_WALKING_LEFT		7

#define MARIO_ANI_DIE				8

#define CENTER_ANI_WALKING			0
#define FONT_WHEEL_ANI_WALKING		1
#define BACK_WHEEL_ANI_WALKING		2
#define FONT_WHEEL_ANI_WALKING		1
#define BACK_WHEEL_ANI_WALKING		2	
#define GUN_ANI_WALKING				3
#define BODY_ANI_WALKING			4

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2

#define SOPHIA_BIG_BBOX_WIDTH  24
#define SOPHIA_BIG_BBOX_HEIGHT 16

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

public: 
	CMario() : CGameObject()
	{
		level = MARIO_LEVEL_BIG;
		untouchable = 0;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};