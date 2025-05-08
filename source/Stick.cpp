#include <math.h>
#include "Stick.h"
#include "Point.h"
#include "Renderer.h"

Stick::Stick(Point& p0, Point& p1, int length)
	: Stick(p0, p1, static_cast<float>(length)) {}

Stick::Stick(Point& p0, Point& p1, float length)
	: p0(p0), p1(p1), length(length) {}

void Stick::Update()
{
	if (!isActive)
	{
		return;
	}

	Vec2 p0Pos = p0.GetPosition();
	Vec2 p1Pos = p1.GetPosition();

	Vec2 diff = p0Pos - p1Pos;
	float dist = sqrtf(diff.x * diff.x + diff.y * diff.y);
	float diffFactor = dist == 0.f ? 0.f : (length - dist) / dist;
	Vec2 offset = diff * diffFactor * 0.5f;

	p0.SetPosition(p0Pos.x + offset.x, p0Pos.y + offset.y);
	p1.SetPosition(p1Pos.x - offset.x, p1Pos.y - offset.y);

	// The scale of the original length this constraint can reach before breaking
	const float elasticity = 11.f;

	if (length * elasticity < dist)
	{
		this->Break();
	}
}

void Stick::Draw(const Renderer* renderer, const Uint32 color) const
{
	if (!isActive)
	{
		return;
	}

	Vec2 p0Pos = p0.GetPosition();
	Vec2 p1Pos = p1.GetPosition();

	bool hasSelectedPoint = p0.GetIsSelected() || p1.GetIsSelected();

	renderer->DrawLine(p0Pos, p1Pos, color);
}

void Stick::Break()
{
	isActive = false;
}
