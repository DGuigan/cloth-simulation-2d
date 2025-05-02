#pragma once

#include "Vec2.h"
#include "SDL.h"

class Renderer;
class Point;
class InputHandler;

class Fan
{
private:
	Vec2 position;
	Vec2 direction;
	float magnitude;
	float angle;
	bool isSelected;

	Uint32 color = 0x03FCA5;
	Uint32 selectedColor = 0x32A8A2;

public:
	Fan(const Vec2 position, const Vec2 direction, const float magnitude, const float angle);

	Fan(const Vec2 position);

	const Vec2& GetPosition() const { return position; };
	const Vec2& GetDirection() const { return direction; };
	const float GetMagnitude() const { return magnitude; };
	const float GetAngle() const { return angle; };

	bool IsPointInFan(const Point& point) const;

	void Draw(const Renderer* renderer) const;

	void Update(InputHandler* inputHandler);
};