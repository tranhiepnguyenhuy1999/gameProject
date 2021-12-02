#pragma once
class Rect
{

public:
	float X;
	float Y;
	float Width;
	float Height;

	Rect();
	Rect(float x, float y, float width, float height) {
		X = x;
		Y = y;
		Width = width;
		Height = height;
	};
	~Rect();
};

