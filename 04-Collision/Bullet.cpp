#include "Bullet.h"

void CBullet::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;
	bottom = y + BULLET_BBOX_HEIGHT;
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}
}

void CBullet::Render()
{
	int ani = BULLET_ANI_WALKING;
	if (state == BULLET_STATE_DIE) {
		ani = BULLET_ANI_DIE;
	}

	animations[ani]->Render(x, y);
	//RenderBoundingBox();
}

void CBullet::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BULLET_STATE_DIE:
		y += BULLET_BBOX_HEIGHT + 1;
		vx = 0;
		vy = 0;
		break;
	case BULLET_STATE_WALKING:
		vx = -BULLET_WALKING_SPEED;
	}
}
