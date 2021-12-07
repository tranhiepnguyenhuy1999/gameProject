#pragma once
#include <d3dx9.h>
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

class ScreenManager
{
	static ScreenManager * __instance;

	float xS;
	float yS;

	public:
	static ScreenManager * GetInstance();
	D3DXVECTOR3 transformPosition(float x, float y);
	void setScreenManagerPosition(float x, float y) { xS = x; yS = y;}
	void getScreenManagerPosition(float &x, float &y) { x = xS; y = yS; }

};

