#pragma once

#include "Vec2.h"
#include "SDL.h"

class Renderer;

class ScoreManager
{
private:
	int targetScore = 0;
	int currentScore = 0;

	Vec2 position;
	Vec2 dimensions;

	Uint32 completedColor = 0x8CFC03;
	Uint32 inProgressColor = 0xFCE803;

public:
	ScoreManager(const int targetScore, const Vec2 position, const Vec2 dimensions);

	void SetCurrentScore(const int inCurrentScore) { currentScore = inCurrentScore; };

	int GetCurrentScore() const { return currentScore; };

	bool GetTargetScoreReached() const { return targetScore <= currentScore; };

	void IncrementCurrentScore(const int scoreDelta);

	void Reset();

	void Draw(const Renderer* renderer);
};