#include <math.h>
#include "Point.h"
#include "Stick.h"
#include "InputHandler.h"
#include "Renderer.h"
#include "Fan.h"
#include "Cloth.h"

#include <iostream>

Point::Point(int x, int y)
	: Point(static_cast<float>(x), static_cast<float>(y)) {}

Point::Point(float x, float y)
{
	pos = prevPos = initPos = Vec2(x, y);
}

void Point::AddStick(Stick* stick)
{
	sticks.push_back(stick);
}

void Point::SetPosition(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

void Point::SetPinned(const bool pinned)
{
	isPinned = pinned;
}

float Point::UpdateSelection(InputHandler* inputHandler)
{
	Vec2 cursorToPosDir = pos - inputHandler->GetMousePosition();
	float cursorToPosDistSquared = cursorToPosDir.LengthSquared();
	float cursorSize = inputHandler->GetCursorSize();
	isSelected = cursorToPosDistSquared < cursorSize * cursorSize;

	if (isSelected)
	{
		return cursorToPosDistSquared;
	}
	return -1.f;
}

void Point::Update(float deltaTime, float drag, const Vec2& acceleration, float elasticity, std::vector<Fan*>* fans, InputHandler* inputHandler, Cloth* cloth, int windowWidth, int windowHeight)
{
	if (!isActive)
	{
		return;
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

	for (const Fan* fan : *fans)
	{
		if (!fan->IsPointInFan(*this))
		{
			continue;
		}

		const Vec2 fanToPoint = (pos - fan->GetPosition());
		const Vec2 fanToPointNormal = fanToPoint.Normalise();
		const float fanToPointDistance = fanToPoint.Length();
		const float fanInfluenceOnPoint = fan->GetMagnitude() - fanToPointDistance;

		const float randomValue = rand() / (float)RAND_MAX;

		prevPos = prevPos - (fanToPointNormal * (fanInfluenceOnPoint * randomValue));
	}

	if (inputHandler->GetRightMouseButtonDown() && isSelected)
	{
		BreakPoint(cloth);
	}

	if (isPinned) {
		pos = initPos;
		return;
	}

	Vec2 newPos = pos + (pos - prevPos) * (1.0f - drag) + acceleration * (1.0f - drag) * deltaTime * deltaTime;
	prevPos = pos;
	pos = newPos;

	KeepInsideView(windowWidth, windowHeight);

	if (hitpoints == 0)
	{
		BreakPoint(cloth);
	}
}

void Point::Draw(const Renderer* renderer) const
{
	if (!isActive)
	{
		return;
	}

	renderer->DrawPoint(GetPosition(), isSelected ? colorWhenSelected : color);
}

void Point::BreakPoint(Cloth* cloth)
{
	for (Stick* stick : sticks)
	{
		if (stick != nullptr)
		{
			stick->Break();
		}
	}
	isActive = false;
	cloth->OnPointRemoved();
}


void Point::KeepInsideView(int windowWidth, int windowHeight)
{
	bool wasOutsideView = false;

	if (pos.x > windowWidth)
	{
		pos.x = static_cast<float>(windowWidth);
		prevPos.x = pos.x;
		wasOutsideView = true;
	}
	else if (pos.x < 0)
	{
		pos.x = 0;
		prevPos.x = pos.x;
		wasOutsideView = true;
	}

	if (pos.y > windowHeight)
	{
		pos.y = static_cast<float>(windowHeight);
		prevPos.y = pos.y;
		wasOutsideView = true;
	}
	else if (pos.y < 0)
	{
		pos.y = 0;
		prevPos.y = pos.y;
		wasOutsideView = true;
	}

	if (wasOutsideView && 0U < hitpoints)
	{
		hitpoints--;
	}
}