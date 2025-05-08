#include "ScoreManager.h"
#include "Renderer.h"

ScoreManager::ScoreManager(const int targetScore, const Vec2 position, const Vec2 dimensions)
	: targetScore(targetScore), position(position), dimensions(dimensions)
{}

void ScoreManager::IncrementCurrentScore(const int scoreDelta)
{
	currentScore += scoreDelta;
	if (targetScore < currentScore)
	{
		currentScore = targetScore;
	}
}

void ScoreManager::Reset()
{
	currentScore = 0;
}

void ScoreManager::Draw(const Renderer* renderer)
{
	const float scoreProgress = static_cast<float>(currentScore) / static_cast<float>(targetScore);
	const float scoreBarHeight = scoreProgress < 1.f ? dimensions.y * scoreProgress : dimensions.y;

	const Uint32 color = GetTargetScoreReached() ? completedColor : inProgressColor;

	renderer->DrawRect(position, dimensions, color, false);
	renderer->DrawRect({ position.x, position.y + (dimensions.y - scoreBarHeight) }, { dimensions.x, scoreBarHeight }, color, true);
}