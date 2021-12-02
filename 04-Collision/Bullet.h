#pragma once
#include "GameObject.h"

#define BULLET_WALKING_SPEED 0.2f;

#define BULLET_BBOX_WIDTH 21
#define BULLET_BBOX_HEIGHT 7

#define BULLET_STATE_WALKING 100
#define BULLET_STATE_DIE 200

#define BULLET_ANI_WALKING 0
#define BULLET_ANI_DIE 1

class CBullet : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();


public:
	virtual void SetState(int state);
};