#include "ScreenManager.h"
ScreenManager * ScreenManager::__instance = NULL;

ScreenManager *ScreenManager::GetInstance()
{
	if (__instance == NULL) __instance = new ScreenManager();
	return __instance;
}

D3DXVECTOR3 ScreenManager::transformPosition(float x, float y)
{
	int xx = x - xS + 30.0f;
	int yy = y - yS +30.0f;
	
	D3DXVECTOR3 p(xx, yy, 0);
	return p;
}