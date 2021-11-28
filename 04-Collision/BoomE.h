
#pragma once
#include "GameObject.h"

#define GOOMBA_WALKING_SPEED 0.05f;

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define BATTRAP_STATE_STATIC 100
#define BATTRAP_STATE_OPEN 200
#define BATTRAP_STATE_DIE 300

#define BATTRAP_ANI_STATIC 0
#define BATTRAP_ANI_OPEN 1
#define BATTRAP_ANI_DIE 2

class BoomE : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	virtual void SetState(int state);
};

