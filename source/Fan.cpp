#include "Fan.h"
#include "Renderer.h"
#include "Point.h"
#include "InputHandler.h"

#include <math.h>

Fan::Fan(const Vec2 position, const Vec2 direction, const float magnitude, const float angle)
	: position(position), direction(direction), magnitude(magnitude), angle(angle)
{}

Fan::Fan(const Vec2 position)
	: position(position), direction({ 1, 0 }), magnitude(100), angle(30)
{}

bool Fan::IsPointInFan(const Point& point) const
{
	const Vec2& pointPos = point.GetPosition();
	const float fanAngle = angle * (M_PI / 180.f);
	const float twoPI = M_PI * 2.f;
	const float fanMagnitudeSquared = magnitude * magnitude;

	const float distanceFromFanSquared =
		((pointPos.x - position.x) * (pointPos.x - position.x)) + ((pointPos.y - position.y) * (pointPos.y - position.y));

	if (distanceFromFanSquared > fanMagnitudeSquared)
	{
		return false;
	}

	float fanAngleWithX = atan2f(direction.y, direction.x);
	if (fanAngleWithX < 0)
	{
		fanAngleWithX = twoPI + fanAngleWithX;
	}

	float fanStartAngle = fanAngleWithX - fanAngle;
	float fanEndAngle = fanAngleWithX + fanAngle;

	if (fanStartAngle < 0)
	{
		fanStartAngle = twoPI + fanStartAngle;
	}

	if (fanEndAngle > twoPI)
	{
		fanEndAngle = fanEndAngle - twoPI;
	}

	float pointAngleInFan = atan2f(pointPos.y - position.y, pointPos.x - position.x);
	if (pointAngleInFan < 0.f)
	{
		pointAngleInFan = twoPI + pointAngleInFan;
	}

	if ((fanStartAngle <= fanEndAngle))
	{
		if ((fanEndAngle < pointAngleInFan) || (pointAngleInFan < fanStartAngle))
		{
			return false;
		}
	}
	else
	{
		if ((pointAngleInFan < fanStartAngle) && (fanEndAngle < pointAngleInFan))
		{
			return false;
		}
	}

	return true;
}


void Fan::Draw(const Renderer* renderer) const
{
	const Vec2 start = position;
	const float angleRadians = angle * (M_PI / 180.f);
	const float coneRadius = tanf(angleRadians) * magnitude;

	float directionAngle = atan2f(direction.y, direction.x);
	float leftVectorAngle = directionAngle + angleRadians;
	float rightVectorAngle = directionAngle - angleRadians;

	const Vec2 leftVector = { cosf(leftVectorAngle), sinf(leftVectorAngle) };
	const Vec2 rightVector = { cosf(rightVectorAngle), sinf(rightVectorAngle) };

	Uint32 drawColor = isSelected ? selectedColor : color;
	renderer->DrawLine(start, start + (leftVector * magnitude), drawColor);
	renderer->DrawLine(start, start + (rightVector * magnitude), drawColor);
}

void Fan::Update(InputHandler* inputHandler)
{
	const Vec2& mousePos = inputHandler->GetMousePosition();

	const float distToMouse = Vec2::Distance(mousePos, position);
	const float fanHitBoxRadius = magnitude * 0.3f;
	// once selected, only release once the mouse is no longer held as the cursor can move too quickly in one frame and lose the fan
	isSelected = ((distToMouse < fanHitBoxRadius) || isSelected) && inputHandler->GetLeftMouseButtonDown();

	if (isSelected)
	{
		position = mousePos;

		const Vec2& arrowKeyStates = inputHandler->GetArrowKeyStates();
		angle += arrowKeyStates.y;

		if (angle > 90.f)
		{
			angle = 90.f;
		}
		else if (angle < 0.f)
		{
			angle = 0.f;
		}

		float directionAngle = atan2f(direction.y, direction.x);
		directionAngle += arrowKeyStates.x * 0.1;

		direction = { cosf(directionAngle), sinf(directionAngle) };
	}
}
