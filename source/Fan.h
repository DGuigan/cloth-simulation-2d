#pragma once

#include "Vec2.h"
#include "SDL.h"

class Renderer;
class Point;
class InputHandler;
class LevelManager;

class Fan
{
private:
	Vec2 position = { 0.f, 0.f };
	Vec2 direction = { 0.f, 0.f };
	float magnitude = 0.f;
	float angle = 0.f;
	bool isSelected = false;

public:
	Fan(const Vec2 position, const Vec2 direction, const float magnitude, const float angle);

	Fan(const Vec2 position);

	const Vec2& GetPosition() const { return position; };
	const Vec2& GetDirection() const { return direction; };
	const float GetMagnitude() const { return magnitude; };
	const float GetAngle() const { return angle; };

	bool IsPointInFan(const Point& point) const;

	void Draw(const Renderer* renderer, LevelManager* levelManager) const;

	void Update(InputHandler* inputHandler);
};