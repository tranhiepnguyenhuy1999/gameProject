#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_GRAVITY			0.1f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300

#define MARIO_WIDTH 14
#define MARIO_HEIGHT 27

class StaticMario : public CGameObject
{
	int state;
public:
	StaticMario();
	~StaticMario();

	void Update(DWORD dt);
	void Render();
};

