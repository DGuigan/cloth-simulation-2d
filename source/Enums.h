#pragma once

#include <SDL.h>

enum class ClothType : Uint8
{
	Rectangular,
	Circular
};

enum class LevelState : Uint8
{
	Design,
	InProgress,
	Completed,
	Failed,
};

enum class RenderElementType : Uint8
{
	UI,
	Background,
	Cloth
};