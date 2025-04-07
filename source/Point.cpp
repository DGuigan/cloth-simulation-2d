#include <math.h>
#include "Point.h"
#include "Stick.h"
#include "InputHandler.h"
#include "Renderer.h"

Point::Point(int x, int y)
	: Point(static_cast<float>(x), static_cast<float>(y)) {}

Point::Point(float x, float y)
{
	pos = prevPos = initPos = Vec2(x, y);
}

void Point::AddStick(Stick* stick, int index)
{
	sticks[index] = stick;
}

void Point::SetPosition(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

void Point::Pin()
{
	isPinned = true;
}

void Point::Update(float deltaTime, float drag, const Vec2& acceleration, float elasticity, InputHandler* inputHandler, int windowWidth, int windowHeight)
{
	Vec2 cursorToPosDir = pos - inputHandler->GetMousePosition();
	float cursorToPosDist = cursorToPosDir.x * cursorToPosDir.x + cursorToPosDir.y * cursorToPosDir.y;
	float cursorSize = inputHandler->GetCursorSize();
	isSelected = cursorToPosDist < cursorSize * cursorSize;

	for (Stick* stick : sticks)
	{
		if (stick != nullptr)
		{
			stick->SetIsSelected(isSelected);
		}
	}

	if (inputHandler->GetLeftMouseButtonDown() && isSelected)
	{
		Vec2 difference = inputHandler->GetMousePosition() - inputHandler->GetPreviousMousePosition();

		if (difference.x > elasticity) difference.x = elasticity;
		if (difference.y > elasticity) difference.y = elasticity;
		if (difference.x < -elasticity) difference.x = -elasticity;
		if (difference.y < -elasticity) difference.y = -elasticity;

		prevPos = pos - difference;
	}

	if (inputHandler->GetRightMouseButtonDown() && isSelected)
	{
		for (Stick* stick : sticks)
		{
			if (stick != nullptr)
			{
				stick->Break();
			}
		}
		isActive = false;
	}

	if (isPinned) {
		pos = initPos;
		return;
	}

	Vec2 newPos = pos + (pos - prevPos) * (1.0f - drag) + acceleration * (1.0f - drag) * deltaTime * deltaTime;
	prevPos = pos;
	pos = newPos;

	KeepInsideView(windowWidth, windowHeight);
}

void Point::Draw(const Renderer* renderer) const
{
	if (!isActive)
	{
		return;
	}

	renderer->DrawPoint(GetPosition(), isSelected ? colorWhenSelected : color);
}

void Point::KeepInsideView(int windowWidth, int windowHeight)
{
	if (pos.x > windowWidth)
	{
		pos.x = static_cast<float>(windowWidth);
		prevPos.x = pos.x;
	}
	else if (pos.x < 0)
	{
		pos.x = 0;
		prevPos.x = pos.x;
	}

	if (pos.y > windowHeight)
	{
		pos.y = static_cast<float>(windowHeight);
		prevPos.y = pos.y;
	}
	else if (pos.y < 0)
	{
		pos.y = 0;
		prevPos.y = pos.y;
	}
}