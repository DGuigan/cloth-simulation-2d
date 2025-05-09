#include "ScoreManager.h"
#include "Renderer.h"
#include "LevelManager.h"
#include "Enums.h"

ScoreManager::ScoreManager(const Vec2 position, const Vec2 dimensions)
	: position(position), dimensions(dimensions)
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

void ScoreManager::Draw(const Renderer* renderer, const LevelManager* levelManager)
{
	const float scoreProgress = static_cast<float>(currentScore) / static_cast<float>(targetScore);
	const float scoreBarHeight = scoreProgress < 1.f ? dimensions.y * scoreProgress : dimensions.y;

	const Uint32 renderColor = levelManager->GetRenderColor(RenderElementType::UI);

	renderer->DrawRect(position, dimensions, renderColor, false);
	renderer->DrawRect({ position.x, position.y + (dimensions.y - scoreBarHeight) }, { dimensions.x, scoreBarHeight }, renderColor, true);
}