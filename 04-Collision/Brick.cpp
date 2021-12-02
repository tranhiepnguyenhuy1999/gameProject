#include "Brick.h"

void CBrick::Render()
{
	for (int i = 0; i < amount; i++)
	{
		if (isHorizontal)
			animations[0]->Render(x + i * BRICK_BBOX_WIDTH, y);
		else
		{
			animations[0]->Render(x, y + i * BRICK_BBOX_HEIGHT);
		}
	}

	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	if (isHorizontal)
	{
		r = x + BRICK_BBOX_WIDTH * amount;
		b = y + BRICK_BBOX_HEIGHT;
	}
	else
	{
		r = x + BRICK_BBOX_WIDTH;
		b = y + BRICK_BBOX_HEIGHT * amount;
	}
}