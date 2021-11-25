#include "StaticMario.h"
#include "Game.h"


StaticMario::StaticMario()
{
}


StaticMario::~StaticMario()
{
}

#include "Mario.h"

void StaticMario::Update(DWORD dt)
{
	vx = MARIO_WALKING_SPEED;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	int BackBufferHeight = CGame::GetInstance()->GetBackBufferHeight();
	
	switch (state)
	{
	case 1:
		y += vx * dt;
		break;
	case 2:
		x += vx * dt;
		break;
	case 3:
		y += (-vx) * dt;
		break;
	case 4:
		x += (-vx) * dt;
		break;
	default:
		break;
	}
	if (y > BackBufferHeight - MARIO_HEIGHT)
	{
		y = (float)BackBufferHeight - MARIO_HEIGHT;
	}
	if (y < 0 )
	{
		y = 0;
	}
	if (x < 0) {

		x = 0;
	}
	if (x > BackBufferWidth - MARIO_WIDTH)
	{
		x = BackBufferWidth - MARIO_WIDTH;
	}
	if (x == 0 && y == 0) {
		state = 1;
	}
	if (x == 0 && y == BackBufferHeight - MARIO_HEIGHT)
	{
		state = 2;
	}
	if (x == BackBufferWidth - MARIO_WIDTH && y == BackBufferHeight - MARIO_HEIGHT)
	{
		state = 3;
	}
	if (x == BackBufferWidth - MARIO_WIDTH && y == 0)
	{
		state = 4;
	}
}

void StaticMario::Render()
{
	int ani;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	int BackBufferHeight = CGame::GetInstance()->GetBackBufferHeight();

	int move = 1;
	if (x >= 0 && x < BackBufferWidth - MARIO_WIDTH && y == 0)
	{
		move = 1;
	}
	if (x >= BackBufferWidth - MARIO_WIDTH && y < BackBufferHeight - MARIO_HEIGHT) {
		x = (float)BackBufferWidth - MARIO_WIDTH;
		move = 2;
	}
	if (x <= BackBufferWidth - MARIO_WIDTH && y >= BackBufferHeight - MARIO_HEIGHT) {
		y = (float)BackBufferHeight - MARIO_HEIGHT;
		move = 3;
	}
	if (x <= 0 && y <= BackBufferHeight - MARIO_HEIGHT && y > 0)
	{
		x = 0;
		move = 4;
	}
	if (y <= 0 && x == 0) {
		y = 0;
		move = 1;
	}

	switch (move)
	{
	case 1:
	case 4:
		ani = MARIO_ANI_WALKING_LEFT;
		break;
	case 2:
	case 3:
		ani = MARIO_ANI_WALKING_RIGHT;
		break;
	default:
		break;
	}

	animations[ani]->Render(x, y);
}

