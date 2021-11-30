#pragma once
#include "Position.h"
#include <d3dx9.h>
class ScreenManager
{
public:
	ScreenManager();
	~ScreenManager();

	D3DXVECTOR3 transformPosition();

};

