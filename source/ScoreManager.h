#pragma once

#include "Vec2.h"
#include "SDL.h"

class Renderer;
class LevelManager;

class ScoreManager
{
private:
	int targetScore = 0;
	int currentScore = 0;

	Vec2 position;
	Vec2 dimensions;

public:
	ScoreManager(const Vec2 position, const Vec2 dimensions);

	void SetCurrentScore(const int inCurrentScore) { currentScore = inCurrentScore; };

	void SetTargetScore(const int inTargetScore) { targetScore = inTargetScore; };

	int GetCurrentScore() const { return currentScore; };

	bool GetTargetScoreReached() const { return targetScore <= currentScore; };

	void IncrementCurrentScore(const int scoreDelta);

	void Reset();

	void Draw(const Renderer* renderer, const LevelManager* levelManager);
};