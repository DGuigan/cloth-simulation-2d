#pragma once

#include "Vec2.h"
#include "SDL.h"

class Renderer;
class LevelManager;

class TimeManager
{
private:
	float timeLimit = 0.f;
	float timeElapsed = 0.f;

	Vec2 position;
	Vec2 dimensions;

public:
	TimeManager(const Vec2 position, const Vec2 dimensions);

	void Update(const float deltaTime) { timeElapsed += deltaTime; };

	void SetTimeLimit(const float inTimeLimit) { timeLimit = inTimeLimit; };

	bool GetTimeLimitReached() const { return timeLimit <= timeElapsed; };

	void Reset() { timeElapsed = 0.f; };

	void Draw(const Renderer* renderer, const LevelManager* levelManager);
};