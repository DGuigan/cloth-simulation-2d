#pragma once

#include <vector>
#include "Vec2.h"

class InputHandler;
class Renderer;
class Point;
class Stick;

class Cloth
{
private:
	Vec2 gravity = { 0.0f, 981.0f };
	float drag = 0.01f;
	float elasticity = 10.0f;

	std::vector<Point*> points;
	std::vector<Stick*> sticks;

public:
	Cloth() = default;
	Cloth(int numColumns, int numRows, int spacing, int startX, int startY);
	~Cloth();

	void Update(Renderer* renderer, InputHandler* inputHandler, float deltaTime);
	void Draw(Renderer* renderer, const bool drawPoints, const bool drawSticks) const;
};
