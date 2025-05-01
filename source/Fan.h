#pragma once

#include "Vec2.h"
#include "SDL.h"

class Renderer;

class Fan
{
private:
	Vec2 position;
	Vec2 direction;
	float magnitude;
	float angle;

	Uint32 color = 0x03FCA5;

public:
	Fan(const Vec2 position, const Vec2 direction, const float magnitude, const float angle);

	const Vec2& GetPosition() const { return position; };
	const Vec2& GetDirection() const { return direction; };
	const float GetMagnitude() const { return magnitude; };
	const float GetAngle() const { return angle; };

	void Draw(const Renderer* renderer) const;
};