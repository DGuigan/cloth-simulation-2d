#pragma once

#include <SDL.h>

enum class ApplicationMode : Uint8
{
	Design,
	Simulate,
};

enum class ClothType : Uint8
{
	Rectangular,
	Circular
};

enum class LevelState : Uint8
{
	Waiting,
	InProgress,
	Completed,
	Failed,
};