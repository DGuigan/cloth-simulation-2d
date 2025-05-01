#include "Fan.h"
#include "Renderer.h"

#include <math.h>

Fan::Fan(const Vec2 position, const Vec2 direction, const float magnitude, const float angle)
	: position(position), direction(direction), magnitude(magnitude), angle(angle)
{}

void Fan::Draw(const Renderer* renderer) const
{
	const Vec2 start = position;
	const float coneRadius = tanf(angle * (M_PI / 180.f)) * magnitude;

	const Vec2 end = position + (direction * magnitude);
	const Vec2 rightVector = { direction.y, -direction.x };

	const Vec2 leftWing = end - (rightVector * coneRadius);
	const Vec2 rightWing = end + (rightVector * coneRadius);

	renderer->DrawLine(start, leftWing, color);
	renderer->DrawLine(start, rightWing, color);
}
