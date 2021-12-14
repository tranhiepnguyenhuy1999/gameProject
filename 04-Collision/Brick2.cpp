#include "Brick2.h"

void CBrick2::Render()
{
	for (int i = 0; i < amount; i++)
	{
		if (isHorizontal)
			animations[0]->Render(x + i * BRICK2_BBOX_WIDTH, y);
		else
		{
			animations[0]->Render(x, y + i * BRICK2_BBOX_HEIGHT);
		}
	}
	RenderBoundingBox();
}

void CBrick2::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;

	if (isHorizontal)
	{
		r = x + BRICK2_BBOX_WIDTH * amount;
		b = y + BRICK2_BBOX_HEIGHT;
	}
	else
	{
		r = x + BRICK2_BBOX_WIDTH;
		b = y + BRICK2_BBOX_HEIGHT * amount;
	}
}