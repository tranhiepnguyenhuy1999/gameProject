#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class CBrick : public CGameObject
{
	int amount;
	bool isHorizontal;
public:
	CBrick(int a, bool direction = true) { amount = a; isHorizontal = direction; };
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

};