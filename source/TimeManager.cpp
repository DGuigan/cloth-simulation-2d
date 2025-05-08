#include "TimeManager.h"
#include "Renderer.h"
#include "LevelManager.h"

TimeManager::TimeManager(const Vec2 position, const Vec2 dimensions)
	: position(position), dimensions(dimensions) {}

void TimeManager::Draw(const Renderer* renderer, const LevelManager* levelManager)
{
	const float timeProgress = 1.f - (timeElapsed / timeLimit);
	const float progressBarHeight = timeProgress < 1.f ? dimensions.y * timeProgress : dimensions.y;

	Uint32 renderColor;

	switch (levelManager->GetCurrentLevelState())
	{
	case (LevelState::Failed):
	{
		renderColor = failedColor;
		break;
	}
	case (LevelState::Completed):
	{
		renderColor = completedColor;
		break;
	}
	default:
	{
		renderColor = inProgressColor;
	}
	}

	renderer->DrawRect(position, dimensions, renderColor, false);
	renderer->DrawRect({ position.x, position.y + (dimensions.y - progressBarHeight) }, { dimensions.x, progressBarHeight }, renderColor, true);
}
