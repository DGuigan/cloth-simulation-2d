#include "InputHandler.h"

void InputHandler::IncreaseCursorSize(float increment)
{
	if (cursorSize + increment > maxCursorSize || cursorSize + increment < minCursorSize)
	{
		return;
	}

	cursorSize += increment;
}

void InputHandler::UpdateMousePosition(int x, int y)
{
	prevPos.x = pos.x;
	prevPos.y = pos.y;
	pos.x = static_cast<float>(x);
	pos.y = static_cast<float>(y);
}

void InputHandler::SetLeftMouseButtonDown(bool isDown)
{
	leftMouseButtonDown = isDown;
	if (isDown)
	{
		leftMouseButtonClickedThisFrame = true;
	}
}

void InputHandler::OnFrameStart()
{
	leftMouseButtonClickedThisFrame = false;
}

void InputHandler::Reset()
{
	leftMouseButtonClickedThisFrame = false;
	selectedFan = nullptr;
}
