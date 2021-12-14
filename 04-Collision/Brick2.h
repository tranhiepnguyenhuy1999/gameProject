#pragma once
#include "GameObject.h"

#define BRICK2_BBOX_WIDTH  32.0f
#define BRICK2_BBOX_HEIGHT 32.0f

class CBrick2 : public CGameObject
{
	int amount;
	bool isHorizontal;
public:
	CBrick2(int a, bool direction = true) { amount = a; isHorizontal = direction; };
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

};