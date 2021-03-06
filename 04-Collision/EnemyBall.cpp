#include "EnemyBall.h"

void EnemyBall::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == ENEMYBALL_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOOMBA_BBOX_HEIGHT;
}

void EnemyBall::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	if (vx < 0 && x < 50) {
		x = 50; vx = -vx;
	}

	if (vx > 0 && x > 160) {
		x = 160; vx = -vx;
	}
}

void EnemyBall::Render()
{
	int ani = ENEMYBALL_ANI_WALKING;
	if (state == ENEMYBALL_STATE_DIE) {
		ani = ENEMYBALL_ANI_DIE;
	}

	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void EnemyBall::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ENEMYBALL_STATE_DIE:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case ENEMYBALL_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
	}
}
