#include "Map.h"

void Map::Render()
{
	animations[0]->Render(x, y);
	//RenderBoundingBox();
}