#include "Sophia.h"

void Sophia::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == BATTRAP_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOOMBA_BBOX_HEIGHT;
}

void Sophia::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	//
}

void Sophia::Render()
{
	//int ani = BATTRAP_ANI_STATIC;

	//switch (state)
	//{
	//case BATTRAP_STATE_OPEN:
	//	ani = BATTRAP_ANI_OPEN;

	//case BATTRAP_STATE_DIE:
	//	ani = BATTRAP_ANI_DIE;
	//default:
	//	break;
	//}

	//animations[ani]->Render(x, y);
	animations[0]->Render(x, y);
	animations[1]->Render(x+9, y);
	animations[2]->Render(x-9, y);
	animations[3]->Render(x+9, y-8);
	animations[4]->Render(x -8, y-8);

	RenderBoundingBox();
}

void Sophia::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BATTRAP_STATE_DIE:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case BATTRAP_STATE_OPEN:
		vx = -GOOMBA_WALKING_SPEED;
	}
}

