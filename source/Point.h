#pragma once

#include <SDL.h>
#include <vector>

#include "Vec2.h"

class InputHandler;
class Renderer;
class Stick;
class Fan;
class Cloth;

class Point
{
private:
	std::vector<Stick*> sticks = { nullptr };

	Vec2 pos;
	Vec2 prevPos;
	Vec2 initPos;
	bool isPinned = false;

	bool isSelected = false;
	bool isActive = true;

	Uint32 color = 0xFF0048E3;
	Uint32 colorWhenSelected = 0xFFCC0000;

	Uint32 hitpoints = 100;

	void KeepInsideView(int width, int height);

public:
	Point() = default;
	Point(int x, int y);
	Point(float x, float y);
	~Point() = default;

	void AddStick(Stick* stick);

	const Vec2& GetPosition() const { return pos; }
	void SetPosition(float x, float y);

	bool GetIsSelected() const { return isSelected; }

	bool GetPinned() const { return isPinned; }
	void SetPinned(const bool pinned);

	float UpdateSelection(InputHandler* inputHandler);
	void Update(float deltaTime, float drag, const Vec2& acceleration, float elasticity, std::vector<Fan*>* fans, InputHandler* inputHandler, Cloth* cloth, int windowWidth, int windowHeight);
	void Draw(const Renderer* renderer) const;

	void BreakPoint(Cloth* cloth);
};