#pragma once

#include "Vec2.h"

class InputHandler
{
private:
	Vec2 pos;
	Vec2 prevPos;

	Vec2 arrowKeyValues;

	float maxCursorSize = 100;
	float minCursorSize = 20;

	float cursorSize = 20;

	bool leftMouseButtonDown = false;
	bool rightMouseButtonDown = false;

	bool leftMouseButtonClickedThisFrame = false;

	bool leftCtrlDown = false;
	bool leftShiftDown = false;

	bool fDown = false;

	bool plusDown = false;
	bool minusDown = false;

public:
	InputHandler() = default;
	~InputHandler() = default;

	const Vec2& GetMousePosition() const { return pos; }
	const Vec2& GetPreviousMousePosition() const { return prevPos; }
	void UpdateMousePosition(int x, int y);

	bool GetLeftMouseButtonDown() const { return leftMouseButtonDown; }
	void SetLeftMouseButtonDown(bool isDown);

	bool GetRightMouseButtonDown() const { return rightMouseButtonDown; }
	void SetRightMouseButtonDown(bool state) { this->rightMouseButtonDown = state; }

	bool GetLeftMouseButtonJustClicked() const { return leftMouseButtonClickedThisFrame; }

	bool GetLeftCtrlDown() const { return leftCtrlDown; }
	void SetLeftCtrlDown(bool isDown) { this->leftCtrlDown = isDown; }

	bool GetLeftShiftDown() const { return leftShiftDown; }
	void SetLeftShiftDown(bool isDown) { this->leftShiftDown = isDown; }

	bool GetFDown() const { return fDown; }
	void SetFDown(bool isDown) { this->fDown = isDown; }

	bool GetPlusDown() const { return plusDown; }
	void SetPlusDown(bool isDown) { this->plusDown = isDown; }

	bool GetMinusDown() const { return minusDown; }
	void SetMinusDown(bool isDown) { this->minusDown = isDown; }

	const Vec2& GetArrowKeyStates() const { return arrowKeyValues; }
	void SetArrowKeyState(const int index, const float value) { arrowKeyValues[index] = value; }

	void IncreaseCursorSize(float increment);
	float GetCursorSize() const { return cursorSize; }

	void OnFrameStart();
};
