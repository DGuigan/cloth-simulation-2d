#pragma once

#include <SDL.h>

class Renderer;
class Point;

class Stick
{
private:
	Point& p0;
	Point& p1;
	float length;

	bool isActive = true;

	Uint32 color = 0xFF0048E3;
	Uint32 colorWhenSelected = 0xFFCC0000;

public:
	Stick(Point& p0, Point& p1, int length);
	Stick(Point& p0, Point& p1, float length);
	~Stick() = default;

	void Update();
	void Draw(const Renderer* renderer) const;
	void Break();
};