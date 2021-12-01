#pragma once
#include <d3dx9.h>

class ScreenManager
{
	static ScreenManager * __instance;

	float xS;
	float yS;

	public:
	static ScreenManager * GetInstance();
	D3DXVECTOR3 transformPosition(float x, float y);
	void setScreenManagerPosition(float x, float y) { xS = x; yS = y;}

};

