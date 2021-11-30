#pragma once
class Position
{
	float x;
	float y;
public:
	Position();
	Position(float xx, float yy) {
		x = xx;
		y = yy;
	};

	~Position();
};

