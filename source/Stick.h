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

public:
	Stick(Point& p0, Point& p1, int length);
	Stick(Point& p0, Point& p1, float length);
	~Stick() = default;

	void Update();
	void Draw(const Renderer* renderer, const Uint32 color) const;
	void Break();
};