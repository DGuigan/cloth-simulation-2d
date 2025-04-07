#pragma once

#include <math.h>

struct Vec2
{
	float x, y;

	Vec2() : x(0), y(0) { }
	Vec2(float x, float y) : x(x), y(y) {}
	~Vec2() = default;

	Vec2 operator + (const Vec2& v) const
	{
		Vec2 result;
		result.x = x + v.x;
		result.y = y + v.y;
		return result;
	}

	Vec2 operator - (const Vec2& v) const
	{
		Vec2 result;
		result.x = x - v.x;
		result.y = y - v.y;
		return result;
	}

	Vec2 operator * (const float n) const
	{
		Vec2 result;
		result.x = x * n;
		result.y = y * n;
		return result;
	}

	Vec2 operator / (const float n) const
	{
		Vec2 result;
		result.x = x / n;
		result.y = y / n;
		return result;
	}

	float LengthSquared() const
	{
		return (x * x) + (y * y);
	}

	float Length() const
	{
		return static_cast<float>(sqrtf(LengthSquared()));
	}

	static float Distance(const Vec2& a, const Vec2& b)
	{
		Vec2 direction = b - a;
		return direction.Length();
	}
};