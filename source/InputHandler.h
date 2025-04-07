#pragma once

#include "Vec2.h"

class InputHandler
{
private:
	Vec2 pos;
	Vec2 prevPos;

	float maxCursorSize = 100;
	float minCursorSize = 20;

	float cursorSize = 20;

	bool leftButtonDown = false;
	bool rightButtonDown = false;

public:
	InputHandler() = default;
	~InputHandler() = default;

	const Vec2& GetMousePosition() const { return pos; }
	const Vec2& GetPreviousMousePosition() const { return prevPos; }
	void UpdateMousePosition(int x, int y);

	bool GetLeftMouseButtonDown() const { return leftButtonDown; }
	void SetLeftMouseButtonDown(bool state) { this->leftButtonDown = state; }

	bool GetRightMouseButtonDown() const { return rightButtonDown; }
	void SetRightMouseButtonDown(bool state) { this->rightButtonDown = state; }

	void IncreaseCursorSize(float increment);
	float GetCursorSize() const { return cursorSize; }
};
